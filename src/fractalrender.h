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

// for apple v other implementations
#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/opencl.h>
#endif

#ifdef HAVE_CL_CL_H
#include <CL/cl.h>
#endif

#ifdef HAVE_OPENCL_CL_H
#include <OpenCL/cl.h>
#endif


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
#endif

int mpi_err, mpi_rank, mpi_numprocs;



#ifdef HAVE_CARGS_H
#include <cargs.h>
#else
#error cargs.h required!
#endif

#ifdef HAVE_PNG_H
#include <png.h>
#endif

#define FR_FAIL exit(3);

#if SIZEOF_DOUBLE != 8
#warning sizeof(double) is not 64 bit!
#endif



#define FR_MAX_PARAMSTRLEN (0x1000)


#ifdef HAVE_GMP
typedef struct fractal_mpf_t {
    
    mpf_t cX, cY, Z;

    mpf_t d_c;

    mpf_t ssp_x, ssp_y;
    
    mpf_t sp_x, sp_y;
    
    mpf_t p_x, p_y;
    mpf_t p_x_s, p_y_s;

    mpf_t tmp; 

} fractal_mpf_t;
#endif


// this keeps track of colors
typedef struct fractal_color_t {

    long numcol;

    // in form FR_COLOR_$X
    long coltype;

    // 3 * numcol, in RGB format
    unsigned char * data;

} fractal_color_t;

// mapping object
typedef struct fractal_img_t {
    long px, py, max_iter;

    // FR_COLOR_X, in tofile.h
    long imgfmt;

    // generic output format, output file
    char *genout, * out;

    fractal_color_t color;

    long prec;

    char *engine;


    long depth;

    char *cX, *cY;

    char *Z;

    // r, g, b
    unsigned char * data;

} fractal_img_t;


#define FR_COLOR_RED                 (0x0001)
#define FR_COLOR_BLUE                (0x0002)
#define FR_COLOR_GREEN               (0x0003)
#define FR_COLOR_BW                  (0x0004)
#define FR_COLOR_RAND                (0x0005)
#define FR_COLOR_MOCHA               (0x0006)

// todo
#define FR_COLOR_FILE                (0x0101)


#include "alloc_lib.h"
#include "tofile.h"
#include "jobs.h"

// defaults that should always be included

#include "io_bmp/io_bmp.h"

#include "engine_c/engine_c.h"

//#include "color_c/color_c.h"



// only if support is enabled

#ifdef HAVE_PNG
#include "io_png/io_png.h"
#endif

#ifdef HAVE_GMP
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
