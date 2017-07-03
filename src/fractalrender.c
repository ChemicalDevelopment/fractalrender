/* fractalrender.c -- source file for a binary

  Copyright 2016-2017 ChemicalDevelopment

  This file is part of the fractalrender project.

  FractalRender source code, as well as any other resources in this project are
free software; you are free to redistribute it and/or modify them under
the terms of the GNU General Public License; either version 3 of the
license, or any later version.

  These programs are hopefully useful and reliable, but it is understood
that these are provided WITHOUT ANY WARRANTY, or MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GPLv3 or email at
<info@chemicaldevelopment.us> for more info on this.

  Here is a copy of the GPL v3, which this software is licensed under. You
can also find a copy at http://www.gnu.org/licenses/.

*/


#include "fractalrender.h"

// offset
#define FRPO "  "

void fr_print_help() {
    printf(PACKAGE_NAME " v" PACKAGE_VERSION "\n");

    printf("Usage: fractalrender [options] -o file...\n");
    printf("Options:\n");
    printf(FRPO "--help                     show help and exit\n");
    printf(FRPO "-w <width>                 set image width to <width>\n");
    printf(FRPO "-h <height>                set image width to <height>\n");
    printf(FRPO "-i <iter>                  set max iterations to <iter>\n");
    printf(FRPO "-z <zoom>                  set image zoom to <zoom>\n");
    printf(FRPO "-x <center x>              set image center to <center x>\n");
    printf(FRPO "-y <center y>              set image center to <center y>\n");
    printf(FRPO FRPO FRPO FRPO "the center of the image is x + iy\n\n");
    printf(FRPO "-c <color>                 set color scheme to <color>\n");
    printf(FRPO "-s                         do simple colorizing\n");
    printf(FRPO FRPO FRPO FRPO "if <color> is a file that ends with .color, the color scheme is read in.\n" FRPO FRPO FRPO FRPO "Otherwise, some builtins, such as `red`, `green`, `mocha`, and `random` can be used.\n\n");
    printf(FRPO "-e <engine>                set compute engine to <engine>\n");
    printf(FRPO FRPO FRPO FRPO "This looks for libraries. If this is failing, try using `-e c` for the default engine.\n\n");
    printf(FRPO "-Z <zoomps>                set animation zoom per second to <zoomps>\n");
    printf(FRPO "-F <fps>                   set animation frames per second to <fps>\n");
    printf(FRPO "-S <sec>                   set animation length to <sec>\n");
    printf(FRPO "-T <animtmp>               set animation temp file format to <animtmp>\n");
    printf(FRPO FRPO FRPO FRPO "example: `-T ~/test/%%d_out.png`, `%%d` is replaced by the index of the frame\n\n");
    printf(FRPO "-o <file>                  save image to <file>\n");
    printf(FRPO FRPO FRPO FRPO "the extension determines which color I/O library to load. \n" FRPO FRPO FRPO FRPO "If your library is not loading, try using `.bmp` files\n\n");
    printf("\n");
    printf("To report bugs, please email:\n");
    printf("<" PACKAGE_BUGREPORT ">\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    srand(time(NULL));

    static struct option long_options[] = {
        // normally, -h and --help,-h would conflict, but getopt_long handles it
        {"help",    optional_argument,       0, 'h'},
        {"color",   required_argument,       0, 'c'},
        {0, 0, 0, 0}
    };


    int log_level = FR_DFT_LOGLVL;

    fr_t fr;

    fr_dim_t dim = (fr_dim_t){ FR_DFT_WIDTH, FR_DFT_HEIGHT };
    fr_col_t col = (fr_col_t){ 30, 0.0, 1.0, false, NULL };
    
    fr_libsearch_t libsearch;

    fr_engine_t fr_engine;
    fr_io_t fr_io;

    fr_libsearch_init(&libsearch);
    fr_libsearch_addpath(&libsearch, "./src/.libs/");
    fr_libsearch_addpath(&libsearch, "");
    fr_libsearch_addpath(&libsearch, "/usr/local/lib/");
    fr_libsearch_addpath(&libsearch, "/usr/lib/");

    char * engine_name = FR_DFT_ENGINE;

    char * file_tmp_out = FR_DFT_ANIMTMP;
    char * file_tmp_out_ext;

    char * file_write_out;

    char * file_out = FR_DFT_O;
    char * file_out_ext;

    char *color_scheme = FR_DFT_COLORSCHEME;

    fr.anim.fps = FR_DFT_ANIMFPS;
    fr.anim.sec = FR_DFT_ANIMSEC;
    fr.anim.zps_str = FR_DFT_ANIMZPS;


    fr.prop.center_x_str = FR_DFT_CENTERX;
    fr.prop.center_y_str = FR_DFT_CENTERX;
    fr.prop.base_zoom_str = FR_DFT_ZOOM;

    fr.prop.er = FR_DFT_ER;

    fr.prop.max_iter = FR_DFT_MAXITER;

    extern int optopt, opterr, optind;
    char c;
    extern char * optarg;
    int long_index = 0;

    struct timeval compute_stime, compute_etime, io_stime, io_etime;

    opterr = 0;

    while ((c = getopt_long(argc, argv, "c:i:Z:F:S:T:x:y:w:h:z:o:e:v:s", long_options, &long_index)) != -1) {
        switch (c) {
            case 's':
                // simple coloring
                col.is_simple = true;
                break;
            case 'o':
                // output file
                file_out = optarg;
                break;
            case 'e':
                // engine
                engine_name = optarg;
                break;
            case 'c':
                //color scheme
                color_scheme = optarg;
                break;
            case 'i':
                fr.prop.max_iter = atoi(optarg);
                break;
            case 'x':
                fr.prop.center_x_str = optarg;
                break;
            case 'y':
                fr.prop.center_y_str = optarg;
                break;
            case 'z':
                fr.prop.base_zoom_str = optarg;
                break;
            case 'v':
                log_level = atoi(optarg);
                break;
            case 'Z':
                fr.anim.zps_str = optarg;
                break;
            case 'F':
                fr.anim.fps = atoi(optarg);
                break;
            case 'S':
                fr.anim.sec = atof(optarg);
                break;
            case 'T':
                file_tmp_out = optarg;
                break;
            case 'w':
                dim.width = atoi(optarg);
                break;
            case 'h':
                if (optarg == NULL || optarg[0] == '-') {
                    // HELP -h or --help was specified
                    fr_print_help();
                } else {
                    // -h was sent with an argument
                    dim.height = atoi(optarg);
                }
                break;
            case '?':
                if (optind == argc || argv[optind][0] == '-') {
                    // help was specified, not -h
                    fr_print_help();
                } else {
                    printf("Argument -%c requires an argument\n", optopt);
                    fr_print_help();
                    FR_FAIL
                }
                break;
            default:
                log_fatal("Unknown getopt return val");
                break;
        }
    }

    log_set_level(log_level);

    fr.anim.zps = atof(fr.anim.zps_str);

    fr_find_engine(&fr_engine, &libsearch, engine_name);

    file_out_ext = fr_get_ext(file_out);
    file_tmp_out_ext = fr_get_ext(file_tmp_out);

    if (STR_EQIC(file_out_ext, "mp4") || STR_EQIC(file_out_ext, "avi")) {
        fr.anim.do_anim = true;
        if (STR_EQIC(file_tmp_out, "@tmp")) {
            int trand = rand() & 0xFFFF;
            struct stat st = { 0 };

            // plenty of room
            file_tmp_out = (char *)malloc(120);
            sprintf(file_tmp_out, "/tmp/fr_out_%04x/", trand);

            if (stat(file_tmp_out, &st) == -1) {
                log_info("creating tmp directory %s", file_tmp_out);
                mkdir(file_tmp_out, 0777);
            }

            sprintf(file_tmp_out, "%s%s", file_tmp_out, "%08d_out.bmp");
        }

        file_tmp_out_ext = fr_get_ext(file_tmp_out);

        fr_find_io(&fr_io, &libsearch, file_tmp_out_ext);

    } else {

        fr.anim.do_anim = false;
        if (file_out_ext == NULL || STR_EQIC(file_out_ext, "")) {
            log_error("unknown extension for file: %s", file_out);
        } else {
            fr_find_io(&fr_io, &libsearch, file_out_ext);
        }
    }


    fr_init(&fr, dim);

    fr_conv_prop(&fr);

    fr_set_col(&fr, col, color_scheme);


    unsigned int total_frames;

    if (fr.anim.do_anim) {
        total_frames = (int)floor(fr.anim.fps * fr.anim.sec);
        file_write_out = (char *)malloc(strlen(file_tmp_out) + 30);
        log_info("going to compute %d frames", total_frames);
    } else {
        total_frames = 1;
        file_write_out = file_out;
    }

    unsigned int total_pixels = total_frames * fr.dim.width * fr.dim.height;

    double compute_time = 0, io_time = 0;

    (*fr_engine.export->fr_init)(&fr);

    int c_frame;

    fr.prop.zoom_str = (char *)malloc(100);

    FILE *out;


    for (c_frame = 0; c_frame < total_frames; ++c_frame) {
        fr.anim.c_frame = c_frame;
        fr.anim.c_time = (double)(1.0 * c_frame) / (1.0 * fr.anim.fps);


        sprintf(fr.prop.zoom_str, "%lf", fr.prop.base_zoom * pow(fr.anim.zps, fr.anim.c_time));

        fr_conv_prop(&fr);


        fr_ctime(&compute_stime);

        (*fr_engine.export->fr_compute)(&fr);

        fr_ctime(&compute_etime);

        compute_time += fr_diffsec(compute_etime, compute_stime);

        if (fr.anim.do_anim) {
            sprintf(file_write_out, file_tmp_out, c_frame + 1);
        }

        log_info("writing to file %s", file_write_out);

        out = fr_sfopen(file_write_out, "wb");

        fr_ctime(&io_stime);

        (*fr_io.export->fr_dump)(&fr, out);

        fr_ctime(&io_etime);

        io_time += fr_diffsec(io_etime, io_stime);

        fclose(out);
    }

    if (fr.anim.do_anim) {
        char * test_ffmpeg_cmd = "ffmpeg -loglevel panic -hide_banner -nostats -h > /dev/null";
        log_info("running system command to test for ffmpeg: '%s'", test_ffmpeg_cmd);
        char * ffmpeg_cmd = (char *)malloc(100 + 100 + strlen(file_tmp_out) + strlen(file_write_out) + strlen(file_out));
        sprintf(ffmpeg_cmd, "ffmpeg -loglevel panic -hide_banner -nostats -y -i %s %s -framerate %d", file_tmp_out, file_out, fr.anim.fps);
        bool have_ffmpeg = system(test_ffmpeg_cmd) == 0;
        if (have_ffmpeg) {
            log_info("running system command to create video: '%s'", ffmpeg_cmd);
            bool ext_code = system(ffmpeg_cmd);
            if (ext_code != 0) {
                log_warn("result was non-zero!");
            }
        } else {
            log_warn("Didn't detect ffmpeg. Run this command: '%s'", ffmpeg_cmd);
        }
    }


    printf("\n");

    log_info("compute took %lfs (%lf Mpixels/s)", compute_time, total_pixels / (1e6 * compute_time));
    log_info("io took %lfs (%lf Mpixels/s)", io_time, total_pixels / (1e6 * io_time));
    log_info("combined took %lfs (%lf Mpixels/s)", compute_time + io_time, total_pixels / (1e6 * (compute_time + io_time)));

    fr_clear(&fr);

    return 0;
}



