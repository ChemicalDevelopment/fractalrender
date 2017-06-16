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

    ret->data = (void *)malloc(px * py * 3);
    
    assert(ret->data != NULL);

    init_frcl(&ret->color);
}


void init_frcl(fractal_color_t *ret) {
  long numcol = cargs_get_int("-ncs");
  ret->numcol = numcol;
  ret->data = (unsigned char *)malloc(ret->numcol * 3);
  long i;
  if (ret->coltype == FR_COLOR_RED) {
    for (i = 0; i < numcol; ++i) {
      ret->data[3 * i + 0] = 255 * i / numcol;
      ret->data[3 * i + 1] = 0;
      ret->data[3 * i + 2] = 0;
    }
  } else if (ret->coltype == FR_COLOR_GREEN) {
    for (i = 0; i < numcol; ++i) {
      ret->data[3 * i + 0] = 0;
      ret->data[3 * i + 1] = 255 * i / numcol;
      ret->data[3 * i + 2] = 0;
    }
  } else if (ret->coltype == FR_COLOR_BLUE) {
    for (i = 0; i < numcol; ++i) {
      ret->data[3 * i + 0] = 0;
      ret->data[3 * i + 1] = 0;
      ret->data[3 * i + 2] = 255 * i / numcol;
    }
  } else if (ret->coltype == FR_COLOR_BW) {
    for (i = 0; i < numcol; ++i) {
      ret->data[3 * i + 0] = 255 * i / numcol;
      ret->data[3 * i + 1] = 255 * i / numcol;
      ret->data[3 * i + 2] = 255 * i / numcol;
    }
  } else if (ret->coltype == FR_COLOR_RAND) {
    for (i = 0; i < numcol; ++i) {
      ret->data[3 * i + 0] = rand() & 0xff;
      ret->data[3 * i + 1] = rand() & 0xff;
      ret->data[3 * i + 2] = rand() & 0xff;
    }
  } else if (ret->coltype == FR_COLOR_MOCHA) {
    double di;
    for (i = 0; i < numcol; ++i) {
      di = (double)i / (numcol);
      ret->data[3 * i + 0] = (long)(floor(di * 255.0)) & 0xff;
      ret->data[3 * i + 1] = (long)(floor(di * di * 255.0)) & 0xff;
      ret->data[3 * i + 2] = (long)(floor(di * di * di * 255.0)) & 0xff;
    }
  } else if (ret->coltype == FR_COLOR_FILE) {
    printf("color files not supported!\n");
    FR_FAIL
  } else {
    printf("unknown color format\n");
    FR_FAIL      
  }
}

