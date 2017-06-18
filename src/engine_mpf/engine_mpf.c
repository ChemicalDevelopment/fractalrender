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

    bool __update_xcache = true;

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

    double zn, di, hue;

    unsigned char *col0, *col1;

    int sci;

    double tmp;

    // use integers
    int er = 4;
    int er2 = er * er;

    for (x = 0; x < ret->px; ++x) {
        mpf_mul_ui(mp->sp_x, mp->d_c, x);
        mpf_add(mp->sp_x, mp->ssp_x, mp->sp_x);

        for (y = 0; y < ret->py; ++y) {

            mpf_mul_ui(mp->sp_y, mp->d_c, y);
            mpf_sub(mp->sp_y, mp->ssp_y, mp->sp_y);

            mpf_set(mp->p_x, mp->sp_x);
            mpf_set(mp->p_y, mp->sp_y);

            mpf_mul(mp->p_x_s, mp->p_x, mp->p_x);
            mpf_mul(mp->p_y_s, mp->p_y, mp->p_y);

            mpf_add(mp->tmp, mp->p_x_s, mp->p_y_s);

            __update_xcache = true;

            for (ci = 1; ci <= ret->max_iter && mpf_cmp_ui(mp->tmp, er2) < 0; ++ci) {
                __update_xcache = true;
                mpf_mul(mp->tmp, mp->p_x, mp->p_y);
                mpf_mul_ui(mp->tmp, mp->tmp, 2);

                mpf_sub(mp->p_x, mp->p_x_s, mp->p_y_s);
                mpf_add(mp->p_x, mp->p_x, mp->sp_x);

                mpf_add(mp->p_y, mp->tmp, mp->sp_y);

                mpf_mul(mp->p_x_s, mp->p_x, mp->p_x);
                mpf_mul(mp->p_y_s, mp->p_y, mp->p_y);

                mpf_add(mp->tmp, mp->p_x_s, mp->p_y_s);
            }

            // TODO:
            // figure out if we need to compute with multiprecision
            di = (double)ci;
            zn = mpf_get_d(mp->tmp);

            if (zn <= er2) {
                hue = 0;
            } else {
                hue = di + 1 - log(fabs(zn)) / log(er2);
            }

            hue *= ret->color.mult;

            while (hue < 0) {
                hue += ret->color.numcol;
            }
            while (hue >= ret->color.numcol) {
                hue -= ret->color.numcol;
            }

            sci = (long)floor(hue);
            tmp = hue - floor(hue);

            col0 = &ret->color.data[3 * sci];
            if (sci >= ret->color.numcol - 1) {
                col1 = &ret->color.data[0];
            } else {
                col1 = &ret->color.data[3 * sci + 3];
            }

            sci = (y * ret->px + x) * 3;

            ret->data[sci + 0] = ((unsigned long)floor(tmp*col1[0]+(1-tmp)*col0[0])) & 0xff;
            ret->data[sci + 1] = ((unsigned long)floor(tmp*col1[1]+(1-tmp)*col0[1])) & 0xff;
            ret->data[sci + 2] = ((unsigned long)floor(tmp*col1[2]+(1-tmp)*col0[2])) & 0xff;

        }
    }
}


