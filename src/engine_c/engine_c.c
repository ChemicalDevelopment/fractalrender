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

    double er = 16.0;
    double er2 = er * er;


    int col_dest;

    for (x = 0; x < ret->px; ++x) {
        for (y = 0; y < ret->py; ++y) {
            col_dest = 3 * (y * ret->px + x);
            xd = ssxd + x * d_xd;
            yd = ssyd + y * d_yd;
            sxd = xd, syd = yd;
            xds = xd * xd, yds = yd * yd;
            
            for (ci = 0; ci < ret->max_iter && xds + yds <= er2; ++ci) {
                tmp = 2 * xd * yd;
                xd = xds - yds + sxd;
                yd = tmp + syd;
                xds = xd * xd;
                yds = yd * yd;
            }

            fr_dft_clr(ret, xds + yds, ci, er2, col_dest);
        }
    }
}


