/* fractalrender_lib.h -- dynamic library loading

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

#ifndef __FRACTALRENDER_LIB_H__
#define __FRACTALRENDER_LIB_H__


// for a dynamically loaded library
typedef struct fr_lib_t {
    // should include 'lib' most of the time.
    // So, for GMP, you would have `libgmp` as `lib_name`, 
    // which would search for `libgmp.{so|dll}` .so  or .dll
    // is chosen based on whether you are compiling on Windows or not.
    char * name;

    // call from dlopen();, representing the library memory location
    void * _lib;

} fr_lib_t;


typedef struct fr_libsearch_t {

    int num_paths;

    // longest strlen, so that we know what to malloc
    size_t biggest_path;

    // array of strings that represent possible paths to search
    char ** paths;

} fr_libsearch_t;



typedef struct fr_io_lib_export_t {

    void (*fr_dump)(fr_t *, FILE *);

} fr_io_lib_export_t;

typedef struct fr_engine_lib_export_t {
    void (*fr_init)(fr_t *);

    void (*fr_compute)(fr_t *);
} fr_engine_lib_export_t;




// a compute engine, such as C, MPF, COMPLEX, or user defined ones.
typedef struct fr_engine_t {
    // actual shared library
    fr_lib_t lib;

    fr_engine_lib_export_t *export;

} fr_engine_t;

// IO library
typedef struct fr_io_t {
    fr_lib_t lib;

    fr_io_lib_export_t *export;

} fr_io_t;


typedef struct fr_interactive_lib_export_t {

    void (*fr_init)(fr_t * fr, fr_engine_t * fr_engine);

    void (*fr_interactive)(fr_t * fr, fr_engine_t * fr_engine);

} fr_interactive_lib_export_t;


// an interactive library
typedef struct fr_interactive_t {

    fr_lib_t lib;

    fr_interactive_lib_export_t *export;

} fr_interactive_t;

void fr_libsearch_init(fr_libsearch_t * libsearch);

void fr_libsearch_addpath(fr_libsearch_t * libsearch, char * path);

void fr_find_interactive(fr_interactive_t * fr_interactive, fr_libsearch_t * libsearch, char * name);

bool fr_find_lib(fr_lib_t * lib, fr_libsearch_t * libsearch, char * name);

void fr_find_io(fr_io_t * fr_io, fr_libsearch_t * libsearch, char * name);

void fr_find_engine(fr_engine_t * fr_engine, fr_libsearch_t * libsearch, char * name);

#endif