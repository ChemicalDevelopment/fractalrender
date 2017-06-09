/* fractalrender.h -- header file for fractalrender.c

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

#ifndef __FRACTALRENDER_H__
#define __FRACTALRENDER_H__


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#undef malloc


#define USE_ENGINE_C

#ifdef USE_GMP
#include <gmp.h>
#endif

#ifdef USE_MPFR
#include <mpfr.h>
#endif

#ifdef USE_MPC
#define USE_ENGINE_MPC
#include <mpc.h>
#endif



#ifdef USE_MPI
#include <mpi.h>
int mpi_err, mpi_rank, mpi_numprocs;
#endif


#if SIZEOF_UNSIGNED_INT == 4
    #define FR_32BIT unsigned int
#elif SIZEOF_UNSIGNED_LONG == 4
    #define FR_32BIT unsigned long
#elif SIZEOF_UNSIGNED_LONG_LONG == 4
    #define FR_32BIT unsigned long long
#else
    #define FR_32BIT unsigned long long
    #warning could not find suitable FR_32BIT (defining as ull)
#endif


#if SIZEOF_UNSIGNED_INT == 8
    #define FR_64BIT unsigned int
#elif SIZEOF_UNSIGNED_LONG == 8
    #define FR_64BIT unsigned long
#elif SIZEOF_UNSIGNED_LONG_LONG == 8
    #define FR_64BIT unsigned long long
#else
    #define FR_64BIT unsigned long long
    #warning could not find suitable FR_64BIT (defining as ull)
#endif


#define FR_32BIT_MAX        ((FR_64BIT)0xFFFFFFFFFFFFFFFF)
#define FR_64BIT_MAX        ((FR_64BIT)0xFFFFFFFFFFFFFFFF)

#define FR_RAW_FORMAT                      (0x1)
#define FR_PNG_FORMAT                      (0x2)


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <cargs.h>
#include <png.h>


// mapping object
typedef struct fractal_img_t {
    long px, py, max_iter;

    // FR_X_FORMAT
    long outfmt;
    // FR_COLOR_X, in tofile.h
    long imgfmt;

    // output file
    char * out;

    long prec;

    char *engine;


    long depth;

    char *cX, *cY;

    char *Z;

    void * data;

} fractal_img_t;


#include "alloc_lib.h"
#include "tofile.h"


// engine code

#ifdef USE_ENGINE_C
#include "engine_c/engine_c.h"
#include "engine_c/color_c.h"
#endif


#ifdef USE_ENGINE_MPC
#include "engine_mpc/engine_mpc.h"
#endif




int main(int argc, char *argv[]);



#endif
