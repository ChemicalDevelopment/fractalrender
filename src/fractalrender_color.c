/* fractalrender_color.c -- common color utilities

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

void fr_col_gen_red(int idx, fr_col_t * fr) {
    fr->in_col[4 * idx + 0] = (255 * (idx + 1)) / fr->col_len;
    fr->in_col[4 * idx + 1] = 0;
    fr->in_col[4 * idx + 2] = 0;
    fr->in_col[4 * idx + 3] = 255;
}

void fr_col_gen_green(int idx, fr_col_t * fr) {
    fr->in_col[4 * idx + 0] = 0;
    fr->in_col[4 * idx + 1] = (255 * (idx + 1)) / fr->col_len;
    fr->in_col[4 * idx + 2] = 0;
    fr->in_col[4 * idx + 3] = 255;
}

void fr_col_gen_blue(int idx, fr_col_t * fr) {
    fr->in_col[4 * idx + 0] = 0;
    fr->in_col[4 * idx + 1] = (255 * (idx + 1)) / fr->col_len;
    fr->in_col[4 * idx + 2] = (255 * (idx + 1)) / fr->col_len;
    fr->in_col[4 * idx + 3] = 255;
}

void fr_col_gen_mocha(int idx, fr_col_t * fr) {
    double port = (double)(idx + 1.0) / fr->col_len;
    fr->in_col[4 * idx + 0] = (int)floor(255 * port);
    fr->in_col[4 * idx + 1] = (int)floor(255 * port * port);
    fr->in_col[4 * idx + 2] = (int)floor(255 * port * port * port);
    fr->in_col[4 * idx + 3] = 255;
}

void fr_col_gen_random(int idx, fr_col_t * fr) {
    fr->in_col[4 * idx + 0] = rand() & 0xFF;
    fr->in_col[4 * idx + 1] = rand() & 0xFF;
    fr->in_col[4 * idx + 2] = rand() & 0xFF;
    fr->in_col[4 * idx + 3] = 255;
}


inline double fr_col_wrap(double fri, int len) {
    return fmod(fmod(fri, len) + len, len);
}

// a generic method for filling in colors that other programs can use
// ci is the current index, zn2 is the square of the absolute value,
// and ri is the result index to store it in
void fr_col_fillinidx(int ci, double zn2, int ri, fr_t * fr) {
    if (fr->col.is_simple) {
        int colci;
        if (zn2 < fr->prop.er2) {
            colci = 0;
        } else {
            colci = (int)floor(ci * fr->col.scale + fr->col.offset);
            colci = fr->dim.byte_depth * (colci % fr->col.col_len);
        }
        fr->bitmap[ri + 0] = fr->col.in_col[colci + 0];
        fr->bitmap[ri + 1] = fr->col.in_col[colci + 1];
        fr->bitmap[ri + 2] = fr->col.in_col[colci + 2];
        fr->bitmap[ri + 3] = fr->col.in_col[colci + 3];
    } else {
        // fractional index
        double fri, mixfactor;
        if (zn2 < fr->prop.er2) {
            // index = 0, because it is inside the set
            fri = 0;
            mixfactor = 0;
            fr->bitmap[ri + 0] = fr->col.in_col[0];
            fr->bitmap[ri + 1] = fr->col.in_col[1];
            fr->bitmap[ri + 2] = fr->col.in_col[2];
            fr->bitmap[ri + 3] = fr->col.in_col[3];
        } else {
            fri = 2 + ci - log(log(zn2)) / log(2.0);
            fri = fri * fr->col.scale + fr->col.offset;
            fri = fmod(fmod(fri, fr->col.col_len) + fr->col.col_len, fr->col.col_len);

            mixfactor = fri - floor(fri);
            int colci0, colci1;

            colci0 = (int)floor(fri);

            if (colci0 >= fr->col.col_len - 1) {
                colci1 = 0;
            } else {
                colci1 = colci0 + 1;
            }

            colci0 *= fr->dim.byte_depth;
            colci1 *= fr->dim.byte_depth;


            fr->bitmap[ri + 0] = (int)floor(mixfactor * fr->col.in_col[colci1 + 0] + (1 - mixfactor) * fr->col.in_col[colci0 + 0]);
            fr->bitmap[ri + 1] = (int)floor(mixfactor * fr->col.in_col[colci1 + 1] + (1 - mixfactor) * fr->col.in_col[colci0 + 1]);
            fr->bitmap[ri + 2] = (int)floor(mixfactor * fr->col.in_col[colci1 + 2] + (1 - mixfactor) * fr->col.in_col[colci0 + 2]);
            fr->bitmap[ri + 3] = (int)floor(mixfactor * fr->col.in_col[colci1 + 3] + (1 - mixfactor) * fr->col.in_col[colci0 + 3]);
            if (fr->bitmap[ri + 3] == 0) {
                fr->bitmap[ri + 3] = 255;
            }

        }
    }
}
