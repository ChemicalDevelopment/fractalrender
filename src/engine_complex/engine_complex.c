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
            
            for (ci = 0; ci < ret->max_iter && cabs(z) <= er; ++ci) {
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

            fr_dft_clr(ret, cabs(z) * cabs(z), ci, er2, col_dest);
        }
    }
}


