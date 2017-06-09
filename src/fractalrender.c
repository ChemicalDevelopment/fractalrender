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

#ifdef USE_MPI
int mpi_err, mpi_rank, mpi_numprocs;

#endif



int main(int argc, char *argv[]) {
    #ifdef USE_MPI
    mpi_err = MPI_Init(&argc, &argv);
    mpi_err = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    mpi_err = MPI_Comm_size(MPI_COMM_WORLD, &mpi_numprocs);
    #endif

    cargs_init(PACKAGE_NAME, PACKAGE_VERSION, argc, argv);

    cargs_add_author("Cade Brown", "cade@chemicaldevelopment.us");

    cargs_add_flag("-4k", NULL, "4k image");

    cargs_add_arg("-d", "--dim", 2, CARGS_ARG_TYPE_INT, "dimensions of image/video");
    cargs_add_default_i("-d", "1920", 0);
    cargs_add_default_i("-d", "1080", 1);



    cargs_add_arg("-p", "--prec", 1, CARGS_ARG_TYPE_INT, "min bits of precision");
    cargs_add_default("-p", "64");


    cargs_add_arg("-e", "--engine", 1, CARGS_ARG_TYPE_STR, "engine (C, GMP, ...)");
    cargs_add_default("-e", "C");

    //cargs_add_arg("-bd", "--bit-depth", 1, CARGS_ARG_TYPE_INT, "bit depth of tests");
    //cargs_add_default("-bd", "8");

    cargs_add_arg("-col", "--Rcolor", 1, CARGS_ARG_TYPE_STR, "red color pattern");
    cargs_add_default("-col", "0");
  
    cargs_add_arg("-Rcol", "--Rcolor", 1, CARGS_ARG_TYPE_STR, "red color pattern");

    cargs_add_arg("-Gcol", "--Gcolor", 1, CARGS_ARG_TYPE_STR, "green color pattern");

    cargs_add_arg("-Bcol", "--Bcolor", 1, CARGS_ARG_TYPE_STR, "blue color pattern");

//    cargs_add_flag("-B", "--binary", "create a binary map");
  
    cargs_add_arg("-i", "--iter", 1, CARGS_ARG_TYPE_INT, "number of iterations to run");
    cargs_add_default("-i", "10");

    cargs_add_arg("-l", "--location", 2, CARGS_ARG_TYPE_STR, "location name, opposed to coordinates");


    // the reason these is STR instead of FLOAT is to support multiprecision
    cargs_add_arg("-c", "--center", 2, CARGS_ARG_TYPE_STR, "x, y coordinates of middle of image");
    cargs_add_default_i("-c", "0.0", 0);
    cargs_add_default_i("-c", "0.0", 1);

    cargs_add_arg("-z", "--zoom", 1, CARGS_ARG_TYPE_STR, "zoom level");
    cargs_add_default("-z", "0.4");

    cargs_add_arg("", NULL, 1, CARGS_ARG_TYPE_STR, "file to save as");
    cargs_add_default("", "out.png");

    cargs_parse();


    long d0 = 0, d1 = 0;
    if (cargs_get_flag("-4k") + cargs_get_flag("-d") > 1) {
        printf("Error: more than 1 size specifier (-4k, -d) used\n");
        exit(3);
    }
    if (cargs_get_flag("-4k")) {
        d0 = 1920 * 2;
        d1 = 1080 * 2;
    } else if (cargs_get_flag("-d")) {
        d0 = cargs_get_int_idx("-d", 0);
        d1 = cargs_get_int_idx("-d", 1);
    } else {
        // still default to -d defaults
        d0 = cargs_get_int_idx("-d", 0);
        d1 = cargs_get_int_idx("-d", 1);
    }


    char * c0 = "", * c1 = "";
    if (cargs_get_flag("-l") + cargs_get_flag("-c") > 1) {
        printf("Error: more than 1 position specifier (-l, -c) used\n");
        exit(3);
    }
  
    if (cargs_get_flag("-l")) {
        char * loc = cargs_get("-l");
        if (strcmp(loc, "elephantvalley") == 0) {
            c0 = ".2821";
            c1 = ".01";
        } else {
            printf("Unknown location: '%s'\n", loc);
            exit(3);
        }
    } else if (cargs_get_flag("-c")) {
        c0 = cargs_get_idx("-c", 0);
        c1 = cargs_get_idx("-c", 1);
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
        #ifndef USE_ENGINE_C
        printf("ERROR: not compiled with support for engine: '%s'\n", engine);
        #endif
    } else if (strcmp(engine, "MPC") == 0) {
        #ifndef USE_ENGINE_MPC
        printf("ERROR: not compiled with support for engine: '%s'\n", engine);
        #endif
    } else {
        printf("Unkown engine: '%s'\n", engine);
    }


    fractal_img_t res;

    res.prec = prec;
    res.engine = engine;

    res.cX = c0;
    res.cY = c1;

    res.Z = zoom;

    init_frit(&res, d0, d1, iter);
    
    if (strcmp(res.engine, "C") == 0) {
        engine_c_fulltest(&res);
    } else if (strcmp(res.engine, "MPC") == 0) {
        engine_mpc_fulltest(&res);
    } else {
        printf("Don't know how to use engine: '%s'\n", res.engine);
    }

    res.out = cargs_get("");

    #define FR_ENDSWITH(a, b) (strlen(a) >= strlen(b) && strcmp(a + strlen(a) - strlen(b), b) == 0)
    if (FR_ENDSWITH(res.out, ".png")) {
        res.outfmt = FR_PNG_FORMAT;
        // TODO: color format here
        res.imgfmt = FR_COLOR_BLUE_ONLY;
    } else if (FR_ENDSWITH(res.out, ".raw")) {
        res.outfmt = FR_RAW_FORMAT;
    } else {
        printf("Unknown format for file: '%s'\n", res.out);
    }

    fractal_to_file(&res);

}


