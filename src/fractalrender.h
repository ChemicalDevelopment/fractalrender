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


#ifdef HAVE_GMP_H
#include <gmp.h>
#endif

#ifdef HAVE_MPFR_H
#include <mpfr.h>
#endif

#ifdef HAVE_MPC_H
#include <mpc.h>
#endif


#ifdef HAVE_MPI_H
#include <mpi.h>
int mpi_err, mpi_rank, mpi_numprocs;
#endif

// for apple v other implementations
#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/opencl.h>
#endif

#ifdef HAVE_CL_CL_H
#include <CL/cl.h>
#endif


#ifdef HAVE_CARGS_H
#include <cargs.h>
#else
#error cargs.h required!
#endif

#ifdef HAVE_PNG_H
#include <png.h>
#endif

#define FR_FAIL exit(3);



#if SIZEOF_UNSIGNED_SHORT == 2
    #define FR_16BIT unsigned short
#elif SIZEOF_UNSIGNED_INT == 2
    #define FR_16BIT unsigned int
#elif SIZEOF_UNSIGNED_LONG == 2
    #define FR_16BIT unsigned long
#elif SIZEOF_UNSIGNED_LONG_LONG == 2
    #define FR_16BIT unsigned long long
#else
    #define FR_16BIT unsigned long long
    #warning could not find suitable FR_32BIT (defining as ull)
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


#define FR_MAX_PARAMSTRLEN (0x1000)


#define FR_16BIT_MAX        ((FR_64BIT)0xFFFF)
#define FR_32BIT_MAX        ((FR_64BIT)0xFFFFFFFF)
#define FR_64BIT_MAX        ((FR_64BIT)0xFFFFFFFFFFFFFFFF)



// a task struct,
// this keeps ahold of timings, and prints conclusions
typedef struct fr_task_t {
    char *name;
    struct timeval stime, etime;
    bool is_done;
} fr_task_t;



// mapping object
typedef struct fractal_img_t {
    long px, py, max_iter;

    // FR_COLOR_X, in tofile.h
    long imgfmt;

    // generic output format, output file
    char *genout, * out;

    long prec;

    char *engine;


    long depth;

    char *cX, *cY;

    char *Z;

    void * data;

} fractal_img_t;


#include "alloc_lib.h"
#include "tofile.h"


// defaults that should always be included

#include "io_raw/io_raw.h"

#include "engine_c/engine_c.h"



// only if support is enabled

#ifdef HAVE_PNG
#include "io_png/io_png.h"
#endif


#ifdef HAVE_MPF
#include "engine_mpf/engine_mpf.h"
#endif

#ifdef HAVE_MPC
#include "engine_mpc/engine_mpc.h"
#endif

#ifdef HAVE_OPENCL
#include "engine_opencl/engine_opencl.h"
#endif



// fractalrender.c defines

void init_fillin();

void init_from_cmdline(fractal_img_t *ret);

void do_engine_test(fractal_img_t * ret);

int main(int argc, char *argv[]);



#endif
