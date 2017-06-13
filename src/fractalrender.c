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

//#endif

// this method 'fills in' defaults to ret, if no flags are set.
void init_fillin(fractal_img_t *ret) {
    if (cargs_get_flag("-A")) {
        // if the user didn't send an output format, fill in the default for -A
        if (!cargs_get_flag("")) {
            ret->out = "%d_out.png";
            ret->genout = "%d_out.png";
        }
    }
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


    char * engine = cargs_get("-e");

    if (strcmp(engine, "C") == 0) {
        // should always have this
    } else if (strcmp(engine, "MPF") == 0) {
        #ifndef HAVE_GMP
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

    init_frit(ret, d0, d1, iter);
    

}

void do_engine_test(fractal_img_t * ret) {
    if (strcmp(ret->engine, "C") == 0) {
        engine_c_fulltest(ret);
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

    #ifdef HAVE_MPI
    mpi_err = MPI_Init(&argc, &argv);
    mpi_err = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    mpi_err = MPI_Comm_size(MPI_COMM_WORLD, &mpi_numprocs);
    printf("process %d/%d started\n", mpi_rank, mpi_numprocs);
    #else
    mpi_err = 0;
    mpi_rank = 0;
    mpi_numprocs = 1;
    #endif

    cargs_init(PACKAGE_NAME, PACKAGE_VERSION, argc, argv);

    cargs_add_author("Cade Brown", "cade@chemicaldevelopment.us");


    cargs_add_arg("-e", "--engine", 1, CARGS_ARG_TYPE_STR, "engine (C, MPF, OPENCL)");
    cargs_add_default("-e", "C");

    cargs_add_arg("--from-raw", NULL, 1, CARGS_ARG_TYPE_STR, "input from .raw files");

    cargs_add_flag("-A", NULL, "create multiple frames");

    cargs_add_arg("-p", "--prec", 1, CARGS_ARG_TYPE_INT, "min bits of precision (only supprted in MPF engine)");
    cargs_add_default("-p", "64");

    cargs_add_arg("-col", "--color", 1, CARGS_ARG_TYPE_STR, "red color pattern");
    cargs_add_default("-col", "0");

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

    cargs_add_arg("-CLdevice", NULL, 1, CARGS_ARG_TYPE_STR, "OpenCL device ('CPU', 'GPU', or 'ALL'");
    cargs_add_default("-CLdevice", "GPU");

    cargs_add_arg("-CLdevicenum", NULL, 1, CARGS_ARG_TYPE_INT, "OpenCL device number");
    cargs_add_default("-CLdevicenum", "1");


    cargs_add_arg("-CLkernel", NULL, 1, CARGS_ARG_TYPE_STR, "OpenCL engine kernel");

    cargs_add_arg("-CLdevice", NULL, 1, CARGS_ARG_TYPE_INT, "OpenCL device number");
    cargs_add_default("-CLdevice", "0");

    cargs_add_arg("-CLsize", NULL, 2, CARGS_ARG_TYPE_INT, "OpenCL local item size");
    cargs_add_default_i("-CLsize", "16", 0);
    cargs_add_default_i("-CLsize", "16", 1);


    cargs_add_arg("", NULL, 1, CARGS_ARG_TYPE_STR, "file to save as");

    // if they don't have -lpng, just compute raw
    #ifdef HAVE_PNG
    cargs_add_default("", "out.png");
    #else
    cargs_add_default("", "out.raw");
    #endif

    cargs_parse();

    fractal_img_t fractal;

    fractal.out = cargs_get("");
    fractal.genout = cargs_get("");

    init_fillin(&fractal);

    if (!cargs_get_flag("--from-raw")) {
        init_from_cmdline(&fractal);
    }

    // choose image format, and maybe change name
    fractal.imgfmt = FR_COLOR_RED_ONLY;

    img_t reti;
    io_init_fractal_to_img(&fractal, &reti);

    figure_out_job(&fractal, &reti);

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


    if (mpi_rank == 0 && cargs_get_flag("-A") && get_format(fractal.genout) != FR_FORMAT_RAW) {
        #define FR_FFMPEG_CMD "ffmpeg -i %s result.mp4 -framerate %s"

        char *cmd = (char *)malloc(strlen(FR_FFMPEG_CMD) + 40 + strlen(fractal.genout));
        sprintf(cmd, FR_FFMPEG_CMD, fractal.genout, cargs_get("--fps"));

        printf("Run the following line to create a video from your created frames\n");
        printf("%s\n", cmd);
    }


    #ifdef HAVE_MPI

    MPI_Finalize();

    #endif

    return 0;
}


