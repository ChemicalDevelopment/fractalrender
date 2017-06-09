/* c_engine/engine_c.c -- C engine for computing fractals

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


void engine_c_fulltest(fractal_img_t * ret) {
    long x, y, ci;

    double ssxd = atof(ret->cX) - 1.0 / atof(ret->Z), ssyd = atof(ret->cY) + ret->py / (ret->px * atof(ret->Z));

    double d_xd = 2.0 / (ret->px * atof(ret->Z)), d_yd = -2.0 / (ret->px * atof(ret->Z));

    double xd, yd, tmp, sxd, syd, xds, yds;

    FR_1BIT * data_1 = (FR_1BIT *)ret->data;
    FR_8BIT * data_8 = (FR_8BIT *)ret->data;
    FR_16BIT * data_16 = (FR_16BIT *)ret->data;
    FR_32BIT * data_32 = (FR_32BIT *)ret->data;
    FR_64BIT * data_64 = (FR_64BIT *)ret->data;
    
    //#pragma omp parallel for
    for (x = 0; x < ret->px; ++x) {
        for (y = 0; y < ret->py; ++y) {
            xd = ssxd + x * d_xd;
            yd = ssyd + y * d_yd;
            sxd = xd, syd = yd;
            xds = xd * xd, yds = yd * yd;
            for (ci = 0; ci < ret->max_iter && xds + yds <= 4.0; ++ci) {
                tmp = 2 * xd * yd;
                xd = xds - yds + sxd;
                yd = tmp + syd;
                xds = xd * xd;
                yds = yd * yd;
            }
            //((char *)ret->data)[y * ret->px + x] = (FR_8BIT_MAX*ci) / ret->max_iter;
            switch (ret->depth) {
                // todo optimize case 1
                case 1: data_1[y * ret->px + x] = ci; break;
                case 8: data_8[y * ret->px + x] = ci; break;
                case 16: data_16[y * ret->px + x] = ci; break;
                case 32: data_32[y * ret->px + x] = ci; break;
                case 64: data_64[y * ret->px + x] = ci; break;
            }
        }
    }
}



