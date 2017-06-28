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
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <complex.h>


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CUDA
#define HAVE_CUDA_H 1
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


#ifdef HAVE_CUDA_H
#include <cuda.h>
#endif

#ifdef HAVE_CUDA_RUNTIME_H
#include <cuda_runtime.h>
#endif

#ifdef HAVE_HELPER_FUNCTIONS_H
#include <helper_functions.h>
#endif

#ifdef HAVE_HELPER_CUDA_H
#include <helper_cuda.h>
#endif


#ifdef HAVE_MPI_H
#include <mpi.h>
#endif

int mpi_err, mpi_rank, mpi_numprocs;

struct timeval scl, ecl;


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

#define CL_LOG_ENV "CL_LOG_ERRORS"

#define FR_MAX_PARAMSTRLEN (0x1000)


#include "fractalrender_defs.h"


#define FR_COLOR_RED                 (0x0001)
#define FR_COLOR_BLUE                (0x0002)
#define FR_COLOR_GREEN               (0x0003)
#define FR_COLOR_BW                  (0x0004)
#define FR_COLOR_RAND                (0x0005)
#define FR_COLOR_MOCHA               (0x0006)
#define FR_COLOR_HAZEOCEAN           (0x0007)

// todo
#define FR_COLOR_FILE                (0x0101)


#include "alloc_lib.h"
#include "tofile.h"
#include "jobs.h"

// defaults that should always be included

#include "io_bmp/io_bmp.h"

#include "engine_c/engine_c.h"
#include "engine_complex/engine_complex.h"

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

#ifdef HAVE_CUDA
#include "engine_cuda/engine_cuda.h"
#include "engine_cuda/engine_cuda_impl.h"
#endif


// fractalrender.c defines

void init_fillin();

void init_from_cmdline(fractal_img_t *ret);

void check_for_unused(fractal_img_t * ret);

void do_engine_test(fractal_img_t * ret);

int main(int argc, char *argv[]);



#endif
