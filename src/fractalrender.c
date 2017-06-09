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

    cargs_add_arg("-d", "--dim", 2, CARGS_ARG_TYPE_INT, "dimensions of image/video");
    cargs_add_default_i("-d", "1920", 0);
    cargs_add_default_i("-d", "1080", 1);


    cargs_add_arg("-bd", "--bit-depth", 1, CARGS_ARG_TYPE_INT, "bit depth of tests");
    cargs_add_default("-bd", "8");

    cargs_add_arg("-i", "--iter", 1, CARGS_ARG_TYPE_INT, "number of iterations to run");
    cargs_add_default("-i", "10");

    // the reason these is STR instead of FLOAT is to support multiprecision
    cargs_add_arg("-c", "--center", 2, CARGS_ARG_TYPE_STR, "x, y coordinates of middle of image");
    cargs_add_default_i("-c", "0.0", 0);
    cargs_add_default_i("-c", "0.0", 1);

    cargs_add_arg("-z", "--zoom", 1, CARGS_ARG_TYPE_STR, "zoom level");
    cargs_add_default("-z", "0.4");

    cargs_add_arg("", NULL, 1, CARGS_ARG_TYPE_STR, "file to save as");
    cargs_add_default("", "out.png");

    cargs_parse();


    fractal_img_t res;
    init_frit(&res, atof(cargs_get_idx("-c", 0)), atof(cargs_get_idx("-c", 1)), atof(cargs_get_idx("-z", 0)), atof(cargs_get_idx("-d", 0)), atof(cargs_get_idx("-d", 1)), atoi(cargs_get("-i")), atoi(cargs_get("-bd")));
    engine_c_fulltest(&res);
    fractal_to_file(&res, cargs_get(""));



}


