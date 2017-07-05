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
    printf(FRPO "-v <verbosity>             set verbosity to <verbosity> (vals are 0,1,2,3,4,5)\n");
    printf(FRPO "-w <width>                 set image width to <width>\n");
    printf(FRPO "-h <height>                set image width to <height>\n");
    printf(FRPO "-i <iter>                  set max iterations to <iter>\n");
    printf(FRPO "-z <zoom>                  set image zoom to <zoom>\n");
    printf(FRPO "-x <center x>              set image center to <center x>\n");
    printf(FRPO "-y <center y>              set image center to <center y>\n");
    printf(FRPO FRPO FRPO FRPO "the center of the image is x + iy\n\n");
    printf(FRPO "-c <color>                 set color scheme to <color>\n");
    printf(FRPO "-E <color offset>          set color scheme offset to <color offset>\n");
    printf(FRPO "-G <color scale>           set color scheme scale to <color scale>\n");
    printf(FRPO "-s                         do simple colorizing\n");
    printf(FRPO FRPO FRPO FRPO "if <color> is a file that ends with .color, the color scheme is read in.\n" FRPO FRPO FRPO FRPO "Otherwise, some builtins, such as `red`, `green`, `mocha`, and `random` can be used.\n\n");
    printf(FRPO "-e <engine>                set compute engine to <engine>\n");
    printf(FRPO "-p <prec>                  set precision to <prec>\n");
    printf(FRPO "-P <prop>                  set prop library to <prop>\n");
    printf(FRPO FRPO FRPO FRPO "This looks for libraries. If this is failing, try using `-e c` for the default engine.\n\n");
    printf(FRPO "-A <args>                  set engine arguments to <args>\n");
    printf(FRPO "-Z <zoomps>                set animation zoom per second to <zoomps>\n");
    printf(FRPO "-F <fps>                   set animation frames per second to <fps>\n");
    printf(FRPO "-S <sec>                   set animation length to <sec>\n");
    printf(FRPO "-T <animtmp>               set animation temp file format to <animtmp>\n");
    printf(FRPO FRPO FRPO FRPO "example: `-T ~/test/%%d_out.png`, `%%d` is replaced by the index of the frame\n\n");
    printf(FRPO "-X                         launch interactive viewer\n");
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

    fr_out_t fr_out;

    fr_engine_t fr_engine;

    fr_prop_lib_t fr_prop_lib;


    fr_dim_t dim = (fr_dim_t){ FR_DFT_WIDTH, FR_DFT_HEIGHT };
    fr_col_t col = (fr_col_t){ 30, 0.0, 1.0, false, NULL };

    fr_libsearch_t libsearch;

    fr_libsearch_init(&libsearch);
    fr_libsearch_addpath(&libsearch, "./src/.libs/");
    fr_libsearch_addpath(&libsearch, "");
    fr_libsearch_addpath(&libsearch, "/usr/local/lib/");
    fr_libsearch_addpath(&libsearch, "/usr/lib/");

    bool do_interactive = false;

    char * engine_name = FR_DFT_ENGINE;

    char * prop_name = FR_DFT_PROP;

    char * engine_args;

    fr_out.file_tmp_out = FR_DFT_ANIMTMP;

    fr_out.file_out = FR_DFT_O;

    char *color_scheme = FR_DFT_COLORSCHEME;

    fr.prop.zoom_str = NULL;


    int prec = 64;

    engine_args = FR_DFT_ENGINEARGS;

    fr.anim.fps = FR_DFT_ANIMFPS;
    fr.anim.sec = FR_DFT_ANIMSEC;

    char * cX = FR_DFT_CENTERX;
    char * cY = FR_DFT_CENTERY;
    char * Z = FR_DFT_ZOOM;
    char * Zps = FR_DFT_ANIMZPS;
    char * er = FR_DFT_ER;

    fr.prop.max_iter = FR_DFT_MAXITER;

    extern int optopt, opterr, optind;
    char c;
    extern char * optarg;
    int long_index = 0;

    opterr = 0;

    while ((c = getopt_long(argc, argv, "c:i:Z:F:S:T:E:G:A:P:p:x:y:w:h:z:o:e:v:sXQ", long_options, &long_index)) != optstop) {
        switch (c) {
            case 's':
                // simple coloring
                col.is_simple = true;
                break;
            case 'o':
                // output file
                fr_out.file_out = optarg;
                break;
            case 'e':
                // engine
                engine_name = optarg;
                break;
            case 'p':
                prec = atoi(optarg);
                break;
            case 'c':
                //color scheme
                color_scheme = optarg;
                break;
            case 'X':
                do_interactive = true;
                break;
            case 'i':
                fr.prop.max_iter = atoi(optarg);
                break;
            case 'x':
                cX = optarg;
                break;
            case 'y':
                cY = optarg;
                break;
            case 'z':
                Z = optarg;
                break;
            case 'v':
                log_level = atoi(optarg);
                break;
            case 'P':
                prop_name = optarg;
                break;
            case 'A':
                engine_args = optarg;
                break;
            case 'Z':
                Zps = optarg;
                break;
            case 'E':
                col.offset = atof(optarg);
                break;
            case 'G':
                col.scale = atof(optarg);
                break;
            case 'F':
                fr.anim.fps = atoi(optarg);
                break;
            case 'S':
                fr.anim.sec = atof(optarg);
                break;
            case 'T':
                fr_out.file_tmp_out = optarg;
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
                log_fatal("Unknown getopt return val: %c", c);
                break;
        }
    }

    optind = 1;

    log_set_level(log_level);

    fr.argc = 1;
    fr.argv = NULL;

    if (!STR_EQ(engine_args, "")) {
        fr.argc++;
    }

    int i, j, k, ct;
    for (i = 0; i < strlen(engine_args); ++i) {
        if (engine_args[i] == ' ') {
            fr.argc++;
        }
    }

    fr.argv = (char **)malloc(sizeof(char *) * fr.argc);
    fr.argv[0] = "__internal_opencl_args";

    ct = 1;

    for (i = 0; i < strlen(engine_args); ++i) {
        k = -1;
        for (j = i; j < strlen(engine_args); ++j) {
            k = j - i + 1;
            if (engine_args[j] == ' ') {
                break;
            }
        }
        if (k == -1) {
            log_fatal("engine args (-A) was given incorrectly");
        } else {
            fr.argv[ct] = (char *)malloc(k);
            strncpy(fr.argv[ct], engine_args + i, k);
            fr.argv[ct][k] = 0;
        }

        i = j;
        ct++;
    }

    log_trace("engine argc: %d", fr.argc);
    for (i = 0; i < fr.argc; ++i) {
        log_trace("engine argv[%d] = %s", i, fr.argv[i]);
    }

    fr.prop.prec = prec;

    fr_find_prop(&fr_prop_lib, &libsearch, prop_name);

    fr_prop_lib.export->fr_prop_init(&fr);

    fr_prop_lib.export->fr_set_prop(&fr, "center_x", cX, 0);
    fr_prop_lib.export->fr_set_prop(&fr, "center_y", cY, 0);

    fr_prop_lib.export->fr_set_prop(&fr, "base_zoom", Z, 0);
    fr_prop_lib.export->fr_set_prop(&fr, "er", er, 0);

    fr_prop_lib.export->fr_set_prop(&fr, "zoom", Z, 0);

    fr_prop_lib.export->fr_set_prop(&fr, "zoomps", Zps, 0);

    fr_find_engine(&fr_engine, &libsearch, engine_name);

    fr_init(&fr, dim);

    log_trace("past start_c_i");

    fr_set_col(&fr, col, color_scheme);

    // switch here

    if (do_interactive) {
        log_trace("using interactive");
        gen_interactive(&fr, &fr_prop_lib,&libsearch, &fr_engine);

    } else {
        log_trace("generating image");
        gen_image(&fr, &fr_prop_lib, &libsearch, &fr_engine, &fr_out);
    }

    return 0;
}
