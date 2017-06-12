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


void engine_mpf_fulltest(fractal_img_t * ret) {
    FR_64BIT x, y, ci;

    bool __update_xcache = true;

    mpf_t cX, cY;
    mpf_init2(cX, ret->prec);
    mpf_init2(cY, ret->prec);
    
    mpf_set_str(cX, ret->cX, 10);
    mpf_set_str(cY, ret->cY, 10);

    mpf_t Z;
    mpf_init2(Z, ret->prec);
    mpf_set_str(Z, ret->Z, 10);


    // delta coordinate
    mpf_t d_c;
    mpf_init2(d_c, ret->prec);
    mpf_set_ui(d_c, 2);
    mpf_div(d_c, d_c, Z);
    mpf_div_ui(d_c, d_c, ret->px);


    mpf_t ssp_x, ssp_y;
    mpf_init2(ssp_x, ret->prec);
    mpf_init2(ssp_y, ret->prec);

    mpf_set(ssp_x, Z);
    mpf_ui_div(ssp_x, 1, ssp_x);
    mpf_sub(ssp_x, cX, ssp_x);


    // ssp_y = cY + py / (Z * px)
    mpf_set(ssp_y, Z);
    mpf_mul_ui(ssp_y, ssp_y, ret->px);
    mpf_ui_div(ssp_y, ret->py, ssp_y);
    mpf_add(ssp_y, cY, ssp_y);


    mpf_t sp_x, sp_y;
    mpf_init2(sp_x, ret->prec);
    mpf_init2(sp_y, ret->prec);

    mpf_t p_x, p_y;
    mpf_init2(p_x, ret->prec);
    mpf_init2(p_y, ret->prec);


    mpf_t p_x_s, p_y_s, tmp;
    mpf_init2(p_x_s, ret->prec);
    mpf_init2(p_y_s, ret->prec);
    mpf_init2(tmp, ret->prec);

    FR_16BIT * data_16 = (FR_16BIT *)ret->data;
    FR_32BIT * data_32 = (FR_32BIT *)ret->data;
    FR_64BIT * data_64 = (FR_64BIT *)ret->data;
    
    for (x = 0; x < ret->px; ++x) {
        mpf_mul_ui(sp_x, d_c, x);
        mpf_add(sp_x, ssp_x, sp_x);

        for (y = 0; y < ret->py; ++y) {

            mpf_mul_ui(sp_y, d_c, y);
            mpf_sub(sp_y, ssp_y, sp_y);

            mpf_set(p_x, sp_x);
            mpf_set(p_y, sp_y);

            mpf_mul(p_x_s, p_x, p_x);
            mpf_mul(p_y_s, p_y, p_y);

            mpf_add(tmp, p_x_s, p_y_s);

            __update_xcache = true;

            for (ci = 0; ci < ret->max_iter && mpf_cmp_ui(tmp, 4) < 0; ++ci) {
                __update_xcache = true;
                mpf_mul(tmp, p_x, p_y);
                mpf_mul_ui(tmp, tmp, 2);

                mpf_sub(p_x, p_x_s, p_y_s);
                mpf_add(p_x, p_x, sp_x);

                mpf_add(p_y, tmp, sp_y);

                mpf_mul(p_x_s, p_x, p_x);
                mpf_mul(p_y_s, p_y, p_y);

                mpf_add(tmp, p_x_s, p_y_s);
            }

            //((char *)ret->data)[y * ret->px + x] = (FR_8BIT_MAX*ci) / ret->max_iter;
            switch (ret->depth) {
                case 16: data_16[y * ret->px + x] = ci; break;
                case 32: data_32[y * ret->px + x] = ci; break;
                case 64: data_64[y * ret->px + x] = ci; break;
            }
        }
    }
    mpf_clear(cX);
    mpf_clear(cY);
    mpf_clear(Z);
    mpf_clear(d_c);

    mpf_clear(ssp_x);
    mpf_clear(ssp_y);

    mpf_clear(sp_x);
    mpf_clear(sp_y);

    mpf_clear(p_x);
    mpf_clear(p_y);

    mpf_clear(p_x_s);
    mpf_clear(p_y_s);

    mpf_clear(tmp);


}



