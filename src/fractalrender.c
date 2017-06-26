/* fractalrender.c -- main program

  Copyright 2016-2017 ChemicalDevelopment

  This file is part of the FractalRender project.

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

//#ifdef HAVE_MPI
int mpi_err, mpi_rank, mpi_numprocs;

struct timeval scl, ecl;


//#endif

// this method 'fills in' defaults to ret, if no flags are set.
void init_fillin(fractal_img_t *ret) {
    int fmt = get_format(cargs_get(""));
    ret->is_anim = fmt == FR_FORMAT_MP4 || fmt == FR_FORMAT_FLV;
    ret->ctime = 0;
}

void init_from_cmdline(fractal_img_t *ret) {

    long d0 = 0, d1 = 0;
    if (cargs_get_flag("-4k") + cargs_get_flag("-d") > 1) {
        printf("Error: more than 1 size specifier (-4k, -d) used\n");
        FR_FAIL
    }
    if (cargs_get_flag("-4k")) {
        d0 = 1920 * 2;
        d1 = 1080 * 2;
    } else {
        d0 = cargs_get_int_idx("-d", 0);
        d1 = cargs_get_int_idx("-d", 1);
    }


    char * c0 = "", * c1 = "";
    if (cargs_get_flag("-l") + cargs_get_flag("-c") > 1) {
        printf("Error: more than 1 position specifier (-l, -c) used\n");
        FR_FAIL
    }
  
    if (cargs_get_flag("-l")) {
        char * loc = cargs_get("-l");
        if (strcmp(loc, "elephantvalley") == 0) {
            c0 = ".2821";
            c1 = ".0100";
        } else {
            printf("Unknown location: '%s'\n", loc);
            FR_FAIL
        }
    } else {
        // still default to -c defaults
        c0 = cargs_get_idx("-c", 0);
        c1 = cargs_get_idx("-c", 1);
    }

    char * zoom;

    if (cargs_get_flag("-z")) {
        zoom = cargs_get("-z");
    } else {
        // still default to -z defaults
        zoom = cargs_get("-z");
    }

    long iter;
    if (cargs_get_flag("-i")) {
        iter = cargs_get_int("-i");
    } else {
        // still default to -z defaults
        iter = cargs_get_int("-i");
    }

    long prec;
    if (cargs_get_flag("-p")) {
        prec = cargs_get_int("-p");
    } else {
        // still default to -z defaults
        prec = cargs_get_int("-p");
    }

    char *ctype_str = cargs_get("-col");
    long color_type;

    if (strcmp(ctype_str, "RED") == 0) {
        color_type = FR_COLOR_RED;
    } else if (strcmp(ctype_str, "GREEN") == 0) {
        color_type = FR_COLOR_GREEN;
    } else if (strcmp(ctype_str, "BLUE") == 0) {
        color_type = FR_COLOR_BLUE;
    } else if (strcmp(ctype_str, "BW") == 0) {
        color_type = FR_COLOR_BW;
    } else if (strcmp(ctype_str, "RAND") == 0) {
        color_type = FR_COLOR_RAND;
    } else if (strcmp(ctype_str, "MOCHA") == 0) {
        color_type = FR_COLOR_MOCHA;
    } else if (strcmp(ctype_str, "HAZEOCEAN") == 0) {
        color_type = FR_COLOR_HAZEOCEAN;
    } else if (get_format(ctype_str) == FR_FORMAT_COLOR) {
        printf("Reading from color file: %s\n", ctype_str);
        color_type = FR_COLOR_FILE;
    } else {
        printf("Don't know how to use color scheme: %s\n", ctype_str);
        FR_FAIL
    }

    char * engine = cargs_get("-e");

    if (strcmp(engine, "C") == 0) {
        // should always have this
    } else if (strcmp(engine, "COMPLEX") == 0) {\
        // should always have this
    } else if (strcmp(engine, "MPF") == 0) {
        #ifndef HAVE_GMPclock_t
        printf("ERROR: not compiled with support for engine: '%s'\n", engine);
        FR_FAIL
        #endif
    } else if (strcmp(engine, "OPENCL") == 0) {
        #ifndef HAVE_OPENCL
        printf("ERROR: not compiled with support for engine: '%s'\n", engine);
        FR_FAIL
        #endif
    } else {
        printf("Unkown engine: '%s'\n", engine);
    }

    ret->prec = prec;
    ret->engine = engine;

    ret->cX = c0;
    ret->cY = c1;

    ret->Z = zoom;

    ret->color.coltype = color_type;

    init_frit(ret, d0, d1, iter);
    

}

// checks and prints any unused inputs.
void check_for_unused(fractal_img_t * ret) {
    if (strcmp(ret->engine, "OPENCL") != 0) {
        if (cargs_get_flag("-CLsize") || cargs_get_flag("-CLdevice") || cargs_get_flag("-CLplatform") || cargs_get_flag("-CLtype") || cargs_get_flag("-CLkernel")) {
            printf("WARNING: Specified `-CL...` arguments without using OpenCL engine\n");
        }
    }
    if (strcmp(ret->engine, "MPF") != 0) {
        if (cargs_get_flag("-p")) {
            printf("WARNING: Specified `-p` precision, but not using MPF engine\n");
        }
    }

    if (!ret->is_anim) {
        if (cargs_get_flag("--sec") || cargs_get_flag("--fps") || cargs_get_flag("--zps") || cargs_get_flag("--anim-tmp")) {
            printf("WARNING: Specifclock_tied animation flags (--sec, --fps, --zps, --anim-tmp), but not creating an animation\n");
        }
    }

}

void do_engine_test(fractal_img_t * ret) {
    if (strcmp(ret->engine, "C") == 0) {
        engine_c_fulltest(ret);
    } else if (strcmp(ret->engine, "COMPLEX") == 0) {
        engine_complex_fulltest(ret);
    } else if (strcmp(ret->engine, "MPF") == 0) {
        printf("ERROR: used do_engine_test for MPF");
        FR_FAIL
    } else if (strcmp(ret->engine, "OPENCL") == 0) {
        #ifdef HAVE_OPENCL
        engine_opencl_fulltest(ret);
        #else
        printf("Trying to use OPENCL engine, but was not compiled with OpenCL support\n");
        FR_FAIL
        #endif
    } else {
        printf("Don't know how to use engine: '%s'\n", ret->engine);
    }
}

int main(int argc, char *argv[]) {


    int to_srand;
    
    setenv("CL_LOG_ERRORS", "stdout", false);

    #ifdef HAVE_MPI
    mpi_err = MPI_Init(&argc, &argv);
    mpi_err = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    mpi_err = MPI_Comm_size(MPI_COMM_WORLD, &mpi_numprocs);
    printf("process %d/%d started\n", mpi_rank, mpi_numprocs);
    if (mpi_rank == 0) {
        to_srand = time(NULL);
    }
    MPI_Bcast(&to_srand, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    #else
    mpi_err = 0;
    mpi_rank = 0;
    mpi_numprocs = 1;
    to_srand = time(NULL);
    
    #endif

    srand(to_srand);

    cargs_init(PACKAGE_NAME, PACKAGE_VERSION, argc, argv);


    char * supported_engines = (char *)malloc(6 * 100);
    sprintf(supported_engines, "%s", "Supported engines: C, COMPLEX");


    #ifdef HAVE_GMP
    sprintf(supported_engines, "%s%s", supported_engines, ", MPF");
    cargs_add_info("Compiled with GMP");
    #endif

    #ifdef HAVE_MPFR
    cargs_add_info("Compiled with MPFR");
    #endif

    #ifdef HAVE_PNG
    cargs_add_info("Compiled with libpng");
    #endif

    #ifdef HAVE_CARGS
    cargs_add_info("Compiled with cargs");
    #endif

    #ifdef HAVE_OPENCL
    sprintf(supported_engines, "%s%s", supported_engines, ", OPENCL");
    cargs_add_info("Compiled with OpenCL");
    #endif

    #ifdef HAVE_MPI
    cargs_add_info("Compiled with MPI");
    #endif

    sprintf(supported_engines, "%s", supported_engines);

    cargs_add_info(supported_engines);



    cargs_add_author("Cade Brown", "cade@chemicaldevelopment.us");


    cargs_add_arg("-e", "--engine", 1, CARGS_ARG_TYPE_STR, "engine (C, MPF, OPENCL)");
    cargs_add_default("-e", "C");

    cargs_add_arg("-col", "--color", 1, CARGS_ARG_TYPE_STR, "color scheme (RED, BW, $FILE, etc)");
    cargs_add_default("-col", "RED");

    cargs_add_flag("-cols", "--color-simple", "use simple color bands, instead of gradient");


    cargs_add_arg("-colm", "--color-mult", 1, CARGS_ARG_TYPE_FLOAT, "change color period");
    cargs_add_default("-colm", "1.0");

    cargs_add_arg("-cold", "--color-disp", 1, CARGS_ARG_TYPE_FLOAT, "change color displacement");
    cargs_add_default("-cold", "0.0");



    cargs_add_arg("-colo", "--color-out", 1, CARGS_ARG_TYPE_FLOAT, "output color file");

    cargs_add_arg("-ncs", "--num-colors", 1, CARGS_ARG_TYPE_INT, "number of colors");
    cargs_add_default("-ncs", "10");
    

    //cargs_add_arg("--from-raw", NULL, 1, CARGS_ARG_TYPE_STR, "input from .raw files");

    cargs_add_flag("--no-image", NULL, "don't create image");
    
    //cargs_add_flag("-A", NULL, "create multiple frames");

    cargs_add_arg("-p", "--prec", 1, CARGS_ARG_TYPE_INT, "min bits of precision (only supprted in MPF engine)");
    cargs_add_default("-p", "64");


    cargs_add_arg("--sec", NULL, 1, CARGS_ARG_TYPE_FLOAT, "seconds");
    cargs_add_default("--sec", "4");

    cargs_add_arg("--fps", NULL, 1, CARGS_ARG_TYPE_FLOAT, "frames per second");
    cargs_add_default("--fps", "10");

    cargs_add_arg("--zps", NULL, 1, CARGS_ARG_TYPE_FLOAT, "zoom per second");
    cargs_add_default("--zps", "1.6");


    cargs_add_arg("-d", "--dim", 2, CARGS_ARG_TYPE_INT, "dimensions of image/video");
    cargs_add_default_i("-d", "1920", 0);
    cargs_add_default_i("-d", "1080", 1);

    cargs_add_flag("-4k", NULL, "4k image");


    cargs_add_arg("-i", "--iter", 1, CARGS_ARG_TYPE_INT, "number of iterations to run");
    cargs_add_default("-i", "25");


    cargs_add_arg("-c", "--center", 2, CARGS_ARG_TYPE_STR, "x, y coordinates of middle of image");
    cargs_add_default_i("-c", "0.0", 0);
    cargs_add_default_i("-c", "0.0", 1);

    cargs_add_arg("-l", "--location", 1, CARGS_ARG_TYPE_STR, "location name, opposed to coordinates");


    cargs_add_arg("-z", "--zoom", 1, CARGS_ARG_TYPE_STR, "zoom level");
    cargs_add_default("-z", "0.4");

    #ifdef HAVE_OPENCL

    cargs_add_flag("-CL32", NULL, "OpenCL use 32 bit");
    cargs_add_flag("-CL64", NULL, "OpenCL force 64 bit");


    cargs_add_arg("-CLplatform", NULL, 1, CARGS_ARG_TYPE_INT, "OpenCL platform number");
    cargs_add_default("-CLplatform", "0");


    cargs_add_arg("-CLtype", NULL, 1, CARGS_ARG_TYPE_STR, "OpenCL device type (CPU, GPU, ALL, ACCELERATOR or DEFAULT)");
    cargs_add_default("-CLtype", "GPU");

    cargs_add_arg("-CLdevice", NULL, 1, CARGS_ARG_TYPE_INT, "OpenCL device number");
    cargs_add_default("-CLdevice", "0");

    cargs_add_arg("-CLkernel", NULL, 1, CARGS_ARG_TYPE_STR, "OpenCL engine kernel");

    cargs_add_arg("-CLdevice", NULL, 1, CARGS_ARG_TYPE_INT, "OpenCL device number");
    cargs_add_default("-CLdevice", "0");

    cargs_add_arg("-CLsize", NULL, 2, CARGS_ARG_TYPE_INT, "OpenCL local item size");
    cargs_add_default_i("-CLsize", "4", 0);
    cargs_add_default_i("-CLsize", "4", 1);

    #endif

    cargs_add_arg("--ffmpeg", NULL, 1, CARGS_ARG_TYPE_STR, "ffmpeg binary");
    cargs_add_default("--ffmpeg", "ffmpeg");

    cargs_add_arg("--anim-tmp", NULL, 1, CARGS_ARG_TYPE_STR, "store temporary files for animation");

    cargs_add_arg("", NULL, 1, CARGS_ARG_TYPE_STR, "file to save as");

    // if they don't have -lpng, just compute bmp files
    #ifdef HAVE_PNG
    cargs_add_default("", "out.png");
    cargs_add_default("--anim-tmp", "/tmp/out_%05d.png");
    #else
    cargs_add_default("", "out.bmp");
    cargs_add_default("--anim-tmp", "/tmp/out_%05d.bmp");
    #endif

    cargs_parse();

    fractal_img_t fractal;

    fractal.out = cargs_get("");
    fractal.tmpout = cargs_get("--anim-tmp");

    init_fillin(&fractal);

    init_from_cmdline(&fractal);

    check_for_unused(&fractal);

    gettimeofday(&scl, NULL);

    figure_out_job(&fractal);

    gettimeofday(&ecl, NULL);

    #ifdef HAVE_OPENCL
    engine_opencl_end();
    #endif

    #ifdef HAVE_MPI

    printf("process %d/%d ended\n", mpi_rank, mpi_numprocs);

    MPI_Barrier(MPI_COMM_WORLD);

    if (mpi_rank == 0) {
        printf("all processes done\n");
    }
    #endif

    fractal.num_pixels_total = fractal.px * fractal.py;

    if (fractal.is_anim) {
        fractal.num_pixels_total *= atof(cargs_get("--sec")) * atof(cargs_get("--fps"));
    }


    if (mpi_rank == 0 && fractal.is_anim) {
        #define FR_FFMPEG_CMD "%s -loglevel panic -hide_banner -nostats -y -i %s %s -framerate %s"
        char *cmd = (char *)malloc(strlen(FR_FFMPEG_CMD) + strlen(cargs_get("")) + strlen(cargs_get("--ffmpeg")) + strlen(fractal.tmpout) + 80);
        sprintf(cmd, FR_FFMPEG_CMD, cargs_get("--ffmpeg"), fractal.tmpout, cargs_get(""), cargs_get("--fps"));

        char *testffmpeg = (char *)malloc(strlen(cargs_get("--ffmpeg")) + 100);
        sprintf(testffmpeg, "%s -loglevel panic -hide_banner -nostats -h > /dev/null", cargs_get("--ffmpeg"));

        // if the ffmpeg binary exists
        if (system(testffmpeg) == 0) {
            int sys_res = system(cmd);
            if (sys_res != 0) {
                printf("ffmpeg (%s) exit code was non-zero!\n", cargs_get("--ffmpeg"));
                printf("Here was the command that was ran:\n%s\n", cmd);
            }
        } else {

            printf("ffmpeg (%s) was not found!\nRun the following line to create a video from your created frames\n", cargs_get("--ffmpeg"));
            printf("%s\n", cmd);
        }
    }


    double elapsed_ns = (ecl.tv_sec - scl.tv_sec) * 1000000.0 + (ecl.tv_usec - scl.tv_usec);
    double mpxl_p_s = fractal.num_pixels_total / (elapsed_ns);

    #ifdef HAVE_MPI

    MPI_Finalize();
    if (mpi_rank == 0) {
        printf("Mpixels/s: %lf\n", mpxl_p_s);
    }

    #else
        printf("Mpixels/s: %lf\n", mpxl_p_s);

    #endif

    return 0;
}


