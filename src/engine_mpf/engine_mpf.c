/* engine_mpc/engine_mpc.c -- MPC engine for computing fractals

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


void engine_mpf_init_mpf(fractal_img_t * ret, fractal_mpf_t *mp) {
    mpf_init2(mp->cX, ret->prec);
    mpf_init2(mp->cY, ret->prec);
    mpf_init2(mp->Z, ret->prec);
    mpf_init2(mp->d_c, ret->prec);
    mpf_init2(mp->ssp_x, ret->prec);
    mpf_init2(mp->ssp_y, ret->prec);
    mpf_init2(mp->sp_x, ret->prec);
    mpf_init2(mp->sp_y, ret->prec);
    mpf_init2(mp->p_x, ret->prec);
    mpf_init2(mp->p_y, ret->prec);
    mpf_init2(mp->p_x_s, ret->prec);
    mpf_init2(mp->p_y_s, ret->prec);
    mpf_init2(mp->tmp, ret->prec);

}

void engine_mpf_clear_mpf(fractal_img_t * ret, fractal_mpf_t *mp) {
    mpf_clear(mp->cX);
    mpf_clear(mp->cY);
    mpf_clear(mp->Z);
    mpf_clear(mp->d_c);

    mpf_clear(mp->ssp_x);
    mpf_clear(mp->ssp_y);

    mpf_clear(mp->sp_x);
    mpf_clear(mp->sp_y);

    mpf_clear(mp->p_x);
    mpf_clear(mp->p_y);

    mpf_clear(mp->p_x_s);
    mpf_clear(mp->p_y_s);

    mpf_clear(mp->tmp);
}


void engine_mpf_fulltest(fractal_img_t * ret, fractal_mpf_t *mp) {
    long x, y, ci;


    mpf_set_str(mp->cX, ret->cX, 10);
    mpf_set_str(mp->cY, ret->cY, 10);

    //mpf_set_str(mp->Z, ret->Z, 10);


    // delta coordinate
    mpf_set_ui(mp->d_c, 2);
    mpf_div(mp->d_c, mp->d_c, mp->Z);
    mpf_div_ui(mp->d_c, mp->d_c, ret->px);


    mpf_set(mp->ssp_x, mp->Z);
    mpf_ui_div(mp->ssp_x, 1, mp->ssp_x);
    mpf_sub(mp->ssp_x, mp->cX, mp->ssp_x);


    // ssp_y = cY + py / (Z * px)
    mpf_set(mp->ssp_y, mp->Z);
    mpf_mul_ui(mp->ssp_y, mp->ssp_y, ret->px);
    mpf_ui_div(mp->ssp_y, ret->py, mp->ssp_y);
    mpf_add(mp->ssp_y, mp->cY, mp->ssp_y);

    int col_dest;

    double tmp;

    // use integers
    int er = 16;
    int er2 = er * er;

    for (x = 0; x < ret->px; ++x) {
        mpf_mul_ui(mp->sp_x, mp->d_c, x);
        mpf_add(mp->sp_x, mp->ssp_x, mp->sp_x);

        for (y = 0; y < ret->py; ++y) {
            col_dest = 3 * (y * ret->px + x);

            mpf_mul_ui(mp->sp_y, mp->d_c, y);
            mpf_sub(mp->sp_y, mp->ssp_y, mp->sp_y);

            mpf_set(mp->p_x, mp->sp_x);
            mpf_set(mp->p_y, mp->sp_y);

            mpf_mul(mp->p_x_s, mp->p_x, mp->p_x);
            mpf_mul(mp->p_y_s, mp->p_y, mp->p_y);

            mpf_add(mp->tmp, mp->p_x_s, mp->p_y_s);

            for (ci = 1; ci <= ret->max_iter && mpf_cmp_ui(mp->tmp, er2) < 0; ++ci) {
                mpf_mul(mp->tmp, mp->p_x, mp->p_y);
                mpf_mul_ui(mp->tmp, mp->tmp, 2);

                mpf_sub(mp->p_x, mp->p_x_s, mp->p_y_s);
                mpf_add(mp->p_x, mp->p_x, mp->sp_x);

                mpf_add(mp->p_y, mp->tmp, mp->sp_y);

                mpf_mul(mp->p_x_s, mp->p_x, mp->p_x);
                mpf_mul(mp->p_y_s, mp->p_y, mp->p_y);

                mpf_add(mp->tmp, mp->p_x_s, mp->p_y_s);
            }


            if (ret->color.is_simple) {
                int color_off;
                if (ci > ret->max_iter) {
                    color_off = 0;
                } else {
                    color_off = 3*((int)floor(ci * ret->color.mult + ret->color.disp) % ret->color.numcol);
                }
                ret->data[col_dest + 0] = ret->color.data[color_off + 0];
                ret->data[col_dest + 1] = ret->color.data[color_off + 1];
                ret->data[col_dest + 2] = ret->color.data[color_off + 2];
                
            } else {
                double zn = mpf_get_d(mp->tmp);
                double hue;
                if (zn <= er2) {
                    hue = 0;
                } else {
                    hue = ci + 1.0 - log(fabs(zn)) / log(er2);
                }

                hue = hue * ret->color.mult + ret->color.disp;
                
                hue = fmod(fmod(hue, ret->color.numcol) + ret->color.numcol, ret->color.numcol);

                tmp = hue - floor(hue);
                int color_off0 = 3 * ((int)floor(hue) % ret->color.numcol);
                int color_off1;
                if (color_off0 >= 3 *(ret->color.numcol - 1)) {
                    color_off1 = 0;
                } else {
                    color_off1 = color_off0 + 3;
                }

                ret->data[col_dest + 0] = ((unsigned char)floor(tmp*ret->color.data[color_off1 + 0]+(1-tmp)*ret->color.data[color_off0 + 0]));
                ret->data[col_dest + 1] = ((unsigned char)floor(tmp*ret->color.data[color_off1 + 1]+(1-tmp)*ret->color.data[color_off0 + 1]));
                ret->data[col_dest + 2] = ((unsigned char)floor(tmp*ret->color.data[color_off1 + 2]+(1-tmp)*ret->color.data[color_off0 + 2]));
            }

        }
    }
}


