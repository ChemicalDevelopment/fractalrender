/* alloc_lib.c -- allocates for fractal images

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


FILE * sfopen(char *fn, char *mode) {
    FILE *ret = fopen(fn, mode);
    if (ret == NULL) {
        printf("ERROR: opening file: %s [mode %s]\n", fn, mode);
        FR_FAIL
    }
    return ret; 
}


void init_frit(fractal_img_t *ret, long px, long py, long max_iter) {
    ret->px = px;
    ret->py = py;

    ret->max_iter = max_iter;

    if (max_iter > FR_32BIT_MAX) {
        ret->depth = 64;
    } else if (max_iter > FR_16BIT_MAX) {
        ret->depth = 32;
        //printf("ERROR: max iterations must be < 2^16\n");
        //FR_FAIL
    } else {
        ret->depth = 16;
    }
    

    ret->data = (void *)malloc(px * py * ret->depth / 8 + 1);
    
    assert(ret->data != NULL);
}

