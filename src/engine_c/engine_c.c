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
    /*
    #define NUM_COL 1000

    unsigned char * cols = (unsigned char *)malloc(NUM_COL * 3);


    for (x = 0; x < NUM_COL * 3; x += 3) {
        cols[x + 0] = rand() & 0xff;
        cols[x + 1] = rand() & 0xff;
        cols[x + 2] = rand() & 0xff;

    }
*/
    long x, y, ci;

    unsigned char *col0, *col1;


    double ssxd = atof(ret->cX) - 1.0 / atof(ret->Z), ssyd = atof(ret->cY) + ret->py / (ret->px * atof(ret->Z));

    double d_xd = 2.0 / (ret->px * atof(ret->Z)), d_yd = -2.0 / (ret->px * atof(ret->Z));

    double xd, yd, tmp, sxd, syd, xds, yds;

    double log_zn;

    long sci;
    double zn, di, hue;

    double er = 4.0;

    double er2 = er * er;


    //#pragma omp parallel for
    for (x = 0; x < ret->px; ++x) {
        for (y = 0; y < ret->py; ++y) {
            xd = ssxd + x * d_xd;
            yd = ssyd + y * d_yd;
            sxd = xd, syd = yd;
            xds = xd * xd, yds = yd * yd;
            
            for (ci = 1; ci <= ret->max_iter && xds + yds <= er2; ++ci) {
                tmp = 2 * xd * yd;
                xd = xds - yds + sxd;
                yd = tmp + syd;
                xds = xd * xd;
                yds = yd * yd;
            }

            zn = xds + yds;
            di = (double)ci;

            if (zn <= er2) {
                hue = 0;
            } else {
                hue = di + 1 - log(fabs(zn)) / log(er2);
            }
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



