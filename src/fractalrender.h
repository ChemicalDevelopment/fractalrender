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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_MPI
#include <mpi.h>
int mpi_err, mpi_rank, mpi_numprocs;
#endif


// engine enum
#define ENGINE_C       (0x01)


#if SIZEOF_UNSIGNED_CHAR == 1
    #define FR_1BIT unsigned char
#else
    #error sizeof(unsigned char) is not 1
#endif


#if SIZEOF_UNSIGNED_CHAR == 1
    #define FR_8BIT unsigned char
#else
    #error could not find suitable FR_8BIT
#endif


#if SIZEOF_UNSIGNED_SHORT == 2
    #define FR_16BIT unsigned short
#elif SIZEOF_UNSIGNED_INT == 2
    #define FR_16BIT unsigned int
#else
    #error could not find suitable FR_16BIT
#endif


#if SIZEOF_UNSIGNED_INT == 4
    #define FR_32BIT unsigned int
#elif SIZEOF_UNSIGNED_LONG == 4
    #define FR_32BIT unsigned long
#elif SIZEOF_UNSIGNED_LONG_LONG == 4
    #define FR_32BIT unsigned long long
#else
    #error could not find suitable FR_32BIT
#endif


#if SIZEOF_UNSIGNED_INT == 8
    #define FR_64BIT unsigned int
    #define FR_VALID_DEPTH(x) ((x) > 0 && ((x) == 1 || (x) == 8 || (x) == 16 || (x) == 32 || (x) == 64))
#elif SIZEOF_UNSIGNED_LONG == 8
    #define FR_64BIT unsigned long
    #define FR_VALID_DEPTH(x) ((x) > 0 && ((x) == 1 || (x) == 8 || (x) == 16 || (x) == 32 || (x) == 64))
#elif SIZEOF_UNSIGNED_LONG_LONG == 8
    #define FR_64BIT unsigned long long
    #define FR_VALID_DEPTH(x) ((x) > 0 && ((x) == 1 || (x) == 8 || (x) == 16 || (x) == 32 || (x) == 64))
#else
    #define FR_64BIT unsigned long long
    #define FR_VALID_DEPTH(x) ((x) > 0 && ((x) == 1 || (x) == 8 || (x) == 16 || (x) == 32))
    #warning could not find suitable FR_64BIT (defining as ull)
#endif

#define FR_1BIT_MAX                        (0x1)
#define FR_8BIT_MAX                       (0xFF)
#define FR_16BIT_MAX                    (0xFFFF)
#define FR_32BIT_MAX                (0xFFFFFFFF)
#define FR_64BIT_MAX        (0xFFFFFFFFFFFFFFFF)



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

    double cX, cY;

    double Z;

    // data depth
    // .data is depth * px * py bytes long

    // depth = 1, each datapoint is a single bit, so a binary mask of points that haven't
    // been ruled out

    // depth = 8, each datapoint is a `char` (8 bit) that stores the fraction (out of 255)
    // that closest resembles iter count

    // depth should probably stay at 16, so that it is a `short`, although if sizeof(short) != 2,
    // program will fail. If this happens, use `int`, it will autodetect
    long depth;
    
    void * data;

} fractal_img_t;


#include "alloc_lib.h"
#include "tofile.h"


// engine code

//#ifdef USE_ENGINE_C
#include "engine_c/engine_c.h"
//#endif



int main(int argc, char *argv[]);



#endif
