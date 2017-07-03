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
    printf(FRPO FRPO "the center of the image is x + iy\n");
    printf(FRPO "-o <file>                  save image to <file>\n");
    printf(FRPO "-s                         do simple colorizing\n");
    printf("\n");
    printf("To report bugs, please email:\n");
    printf("<" PACKAGE_BUGREPORT ">\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        // normally, -h and --help,-h would conflict, but getopt_long handles it
        {"help",    optional_argument,       0, 'h'},
        {"color",   required_argument,       0, 'c'},
        {0, 0, 0, 0}
    };

    log_set_level(LOG_TRACE);

    fr_t fr;
    fr_dim_t dim = (fr_dim_t){ FR_DFT_WIDTH, FR_DFT_HEIGHT };
    fr_col_t col = (fr_col_t){ 30, 0.0, 1.0, false, NULL };
    
    fr_libsearch_t libsearch;

    fr_engine_t fr_engine;
    fr_io_t fr_io;

    fr_libsearch_init(&libsearch);
    fr_libsearch_addpath(&libsearch, "");
    fr_libsearch_addpath(&libsearch, "./src/.libs/");
    fr_libsearch_addpath(&libsearch, "/usr/local/lib/");
    fr_libsearch_addpath(&libsearch, "/usr/lib/");

    char * engine_name = FR_DFT_ENGINE;

    char * file_out = FR_DFT_O;
    char * file_out_ext;

    char *color_scheme = FR_DFT_COLORSCHEME;

    fr.prop.center_x_str = FR_DFT_CENTERX;
    fr.prop.center_y_str = FR_DFT_CENTERX;
    fr.prop.zoom_str = FR_DFT_ZOOM;

    fr.prop.er = FR_DFT_ER;

    fr.prop.max_iter = FR_DFT_MAXITER;

    extern int optopt, opterr, optind;
    char c;
    extern char * optarg;
    int long_index = 0;

    struct timeval compute_stime, compute_etime, io_stime, io_etime;

    opterr = 0;

    while ((c = getopt_long(argc, argv, "c:i:x:y:w:h:z:o:e:s", long_options, &long_index)) != -1) {
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
                fr.prop.zoom_str = optarg;
                break;
            case 'w':
                dim.width = atoi(optarg);
                break;
            case 'h':
                if (optarg == NULL && (optind == argc || argv[optind][0] == '-')) {
                    // HELP -h or --help was specified
                    fr_print_help();
                } else {
                    // -h was sent with an argument
                    printf("%s\n", optarg);
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


    fr_find_engine(&fr_engine, &libsearch, engine_name);

    file_out_ext = fr_get_ext(file_out);

    if (file_out_ext == NULL || STR_EQIC(file_out_ext, "")) {
        log_error("unknown extension for file: %s", file_out);
    } else {
        fr_find_io(&fr_io, &libsearch, file_out_ext);
    }


    fr_init(&fr, dim);

    fr_conv_prop(&fr);

    fr_set_col(&fr, col, color_scheme);

    (*fr_engine.export->fr_init)(&fr);

    fr_ctime(&compute_stime);

    (*fr_engine.export->fr_compute)(&fr);

    fr_ctime(&compute_etime);

    //fr_io_bmp_dump(&fr, out);


    log_info("writing to file %s", file_out);

    FILE *out = fr_sfopen(file_out, "wb");


    fr_ctime(&io_stime);

    (*fr_io.export->fr_dump)(&fr, out);

    fr_ctime(&io_etime);

    fclose(out);

    double compute_time = fr_diffsec(compute_etime, compute_stime);
    double io_time = fr_diffsec(io_etime, io_stime);
    unsigned int total_pixels = fr.dim.width * fr.dim.height;

    printf("\n");

    log_info("compute took %lfs (%lf Mpixels/s)", compute_time, total_pixels / (1e6 * compute_time));
    log_info("io took %lfs (%lf Mpixels/s)", io_time, total_pixels / (1e6 * io_time));
    log_info("combined took %lfs (%lf Mpixels/s)", compute_time + io_time, total_pixels / (1e6 * (compute_time + io_time)));

    fr_clear(&fr);

    return 0;
}



