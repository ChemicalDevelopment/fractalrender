/* io_raw.c -- .raw format input/output

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


void color_c_fractal_to_img(fractal_img_t * ret, img_t * reti) {

    long x, y;

    int fr_ctype = ret->imgfmt;

    FR_16BIT * data_16 = (FR_16BIT *)ret->data;
    FR_32BIT * data_32 = (FR_32BIT *)ret->data;
    FR_64BIT * data_64 = (FR_64BIT *)ret->data;

    FR_64BIT dataxy;

    char * row;

    for (y = 0; y < ret->py; y++) {
        row = &reti->data[y * ret->px * 3];
        for (x = 0; x < ret->px; x++) {
            switch (ret->depth) {
                case 16: dataxy = data_16[y * ret->px + x]; break;
                case 32: dataxy = data_32[y * ret->px + x]; break;
                case 64: dataxy = data_64[y * ret->px + x]; break;
            }
            if (fr_ctype == FR_COLOR_RED_ONLY || fr_ctype == FR_COLOR_GREEN_ONLY || fr_ctype == FR_COLOR_BLUE_ONLY || fr_ctype == FR_COLOR_BW_LIN) {
                double c_xy = (double)dataxy / ret->max_iter;
                if (fr_ctype == FR_COLOR_RED_ONLY) {
                    row[3*x + 0] = (int)floor(255 * c_xy);
                    row[3*x + 1] = 0;
                    row[3*x + 2] = 0;
                } else if (fr_ctype == FR_COLOR_GREEN_ONLY) {
                    row[3*x + 0] = 0;
                    row[3*x + 1] = (int)floor(255 * c_xy);
                    row[3*x + 2] = 0;
                } else if (fr_ctype == FR_COLOR_BLUE_ONLY) {
                    row[3*x + 0] = 0;
                    row[3*x + 1] = 0;
                    row[3*x + 2] = (int)floor(255 * c_xy);
                } else if (fr_ctype == FR_COLOR_BW_LIN) {
                    row[3*x + 0] = (int)floor(255 * c_xy);
                    row[3*x + 1] = (int)floor(255 * c_xy);
                    row[3*x + 2] = (int)floor(255 * c_xy);
                }
            } else if (fr_ctype == FR_COLOR_BINARY) {
                row[3*x+0] = 255 * (dataxy == ret->max_iter);
                row[3*x+1] = 255 * (dataxy == ret->max_iter);
                row[3*x+2] = 255 * (dataxy == ret->max_iter);
            } else {
                printf("Unknown fractalrender color scheme!\n");
                exit(3);
            }
        }
    }

}

