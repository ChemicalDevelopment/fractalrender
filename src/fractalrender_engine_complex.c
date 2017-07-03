/* fractalrender_engine_c.c -- C engine

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

#include "fractalrender_engine_complex.h"


fr_engine_lib_export_t fr_engine_export = {
    fr_engine_complex_init,
    fr_engine_complex_compute
};

#define __FR_COMPLEX_NORMSQR(x) creal(x) * creal(x) + cimag(x) * cimag(x)
#define FR_COMPLEX_NORMSQR(x) (__FR_COMPLEX_NORMSQR((x)))


void fr_engine_complex_init(fr_t * fr) {
    log_info("complex engine initialized");
}

void fr_engine_complex_compute(fr_t * fr) {
    log_info("complex engine computing started");

    int px, py;

    // result index to store in
    int ri;

    // current iter
    int ci;

    // top left corner
    double complex TL_corner = (fr->prop.center_x - 1.0 / fr->prop.zoom) + I * (fr->prop.center_y + fr->dim.height / (fr->dim.width * fr->prop.zoom));

    // complex numbers
    double complex z, c;

    // delta per pixel, this is subtracted from Y, but added to X
    double delta_ppx = 2.0 / (fr->dim.width * fr->prop.zoom);

    for (px = 0; px < fr->dim.width; ++px) {
        for (py = 0; py < fr->dim.height; ++py) {
            ri = 4 * (py * fr->dim.width + px);

            c = TL_corner + (px * delta_ppx) - I * (py * delta_ppx);
            z = c;

            for (ci = 0; FR_COMPLEX_NORMSQR(z) <= fr->prop.er2 && ci < fr->prop.max_iter; ++ci) {
                z = z * z + c;
            }

            fr_col_fillinidx(ci, FR_COMPLEX_NORMSQR(z), ri, fr);

        }
    }
    log_info("complex engine computing ended");
}

