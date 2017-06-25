/* engine_complex/engine_complex.c -- C engine for computing fractals, with complex numbers

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


inline double complex cpowi(double complex a, int b) {
    double complex pa = 1;
    while (b > 0) {
        pa = pa * a;
        b--;
    }
    return pa;    
}


double complex cgamma(double complex a) {
    if (creal(a) < 4) {
        return cgamma(a + 1) / a;
    }
    double complex lga = clog(a);
    double complex a_n2 = 1 / (a * a);
    double complex a_k_n1 = 1 / a;
    double complex _ea = a * lga;
    _ea = _ea - a;
    _ea = _ea - clog(a / (2 * 3.141592653589793238)) / 2;
    _ea = _ea + a_k_n1 / 12;
    a_k_n1 = a_k_n1 * a_n2;
    _ea = _ea - a_k_n1 / 360;
    a_k_n1 = a_k_n1 * a_n2;
    _ea = _ea + a_k_n1 / 1260;
    return cexp(_ea);
}

void engine_complex_fulltest(fractal_img_t * ret) {

    long x, y, ci;

    double complex base_complex = atof(ret->cX) - 1.0 / atof(ret->Z) + I * (atof(ret->cY) + ret->py / (ret->px * atof(ret->Z)));
    double complex base_add_complex = 2.0 / (ret->px * atof(ret->Z)) + I * (-2.0 / (ret->px * atof(ret->Z)));

    double complex c, z;

    double er = 16.0;
    double er2 = er * er;

    double tmp;
    int col_dest;

    for (x = 0; x < ret->px; ++x) {
        for (y = 0; y < ret->py; ++y) {
            col_dest = 3 * (y * ret->px + x);

            c = (creal(base_complex) + creal(base_add_complex) * x) + I * (cimag(base_complex) + cimag(base_add_complex) * y);
            z = c;
            
            for (ci = 1; ci <= ret->max_iter && cabs(z) <= er; ++ci) {
                z = z * z + c;

                //z = (1 + 2 * cpowi(z, 3)) / (3 * cpowi(z, 2));
                //z = cexp(z) + c;
                //z = cgamma(z) + c;
                //z = cpow(z, 2 + I) + c;
                //z = cpow(z, 2.0 + ret->ctime / 2.0) + c;
                //z = cpowi(z, 6) + c;
                //z = z * z + .5 + .5 * I;
                //z = z * z + (1 - 1.61803398875);
                //z = z * z + -1.75;
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
                double zn = cabs(z) * cabs(z);
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


