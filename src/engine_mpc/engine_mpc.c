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


void engine_mpc_fulltest(fractal_img_t * ret) {
    FR_64BIT x, y, ci;

    mpfr_t cX, cY;
    mpfr_init2(cX, ret->prec);
    mpfr_init2(cY, ret->prec);
    
    mpfr_set_str(cX, ret->cX, 10, MPFR_RNDN);
    mpfr_neg(cX, cX, MPFR_RNDN);
    mpfr_set_str(cY, ret->cY, 10, MPFR_RNDN);

    mpfr_t Z;
    mpfr_init2(Z, ret->prec);
    mpfr_set_str(Z, ret->Z, 10, MPFR_RNDN);

    // delta coordinate
    mpfr_t d_c;
    mpfr_init2(d_c, ret->prec);
    mpfr_set_ui(d_c, 2, MPFR_RNDN);
    mpfr_div(d_c, d_c, Z, MPFR_RNDN);
    mpfr_div_ui(d_c, d_c, ret->px, MPFR_RNDN);

    // start point, point
    mpc_t ssp, sp, p;
    mpc_init2(ssp, ret->prec);
    mpc_init2(sp, ret->prec);
    mpc_init2(p, ret->prec);

    mpfr_set(mpc_realref(ssp), Z, MPFR_RNDN);
    mpfr_ui_div(mpc_realref(ssp), 1, mpc_realref(ssp), MPFR_RNDN);
    mpfr_sub(mpc_realref(ssp), cX, mpc_realref(ssp), MPFR_RNDN);


    mpfr_set(mpc_imagref(ssp), Z, MPFR_RNDN);
    mpfr_mul_ui(mpc_imagref(ssp), mpc_imagref(ssp), ret->px, MPFR_RNDN);
    mpfr_ui_div(mpc_imagref(ssp), ret->py, mpc_imagref(ssp), MPFR_RNDN);
    mpfr_add(mpc_imagref(ssp), cY, mpc_imagref(ssp), MPFR_RNDN);

    mpfr_t norm_p;
    mpfr_init2(norm_p, ret->prec);

    
    FR_16BIT * data_16 = (FR_16BIT *)ret->data;
    FR_32BIT * data_32 = (FR_32BIT *)ret->data;
    FR_64BIT * data_64 = (FR_64BIT *)ret->data;
    //mpfr_printf("%Rf,%Rf\n", mpc_realref(ssp), mpc_imagref(ssp));
    
    for (x = 0; x < ret->px; ++x) {
        for (y = 0; y < ret->py; ++y) {
            mpfr_mul_ui(mpc_realref(sp), d_c, ret->px-x-1, MPFR_RNDN);
            mpfr_add(mpc_realref(sp), mpc_realref(ssp), mpc_realref(sp), MPFR_RNDN);
            
            mpfr_mul_ui(mpc_imagref(sp), d_c, y, MPFR_RNDN);
            mpfr_sub(mpc_imagref(sp), mpc_imagref(ssp), mpc_imagref(sp), MPFR_RNDN);

            mpc_set(p, sp, MPFR_RNDN);


            for (ci = 0; ci < ret->max_iter; ++ci) {
                mpc_sqr(p, p, MPFR_RNDN);
                mpc_sub(p, p, sp, MPFR_RNDN);
                mpc_norm(norm_p, p, MPFR_RNDN);
                if (mpfr_cmp_ui(norm_p, 4) > 0) {
                    break;
                }
            }
            //((char *)ret->data)[y * ret->px + x] = (FR_8BIT_MAX*ci) / ret->max_iter;
            switch (ret->depth) {
                case 16: data_16[y * ret->px + x] = ci; break;
                case 32: data_32[y * ret->px + x] = ci; break;
                case 64: data_64[y * ret->px + x] = ci; break;
            }
        }
    }
    mpfr_clear(cX);
    mpfr_clear(cY);
    mpfr_clear(Z);
    mpfr_clear(d_c);

    mpc_clear(ssp);
    mpc_clear(sp);
    mpc_clear(p);

    mpfr_clear(norm_p);
}



