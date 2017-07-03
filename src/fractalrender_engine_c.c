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

#include "fractalrender_engine_c.h"



fr_engine_lib_export_t fr_engine_export = {
    fr_engine_c_init,
    fr_engine_c_compute
};

void fr_engine_c_init(fr_t * fr) {
    log_debug("c engine initialized");
}

void fr_engine_c_compute(fr_t * fr) {
    log_debug("c engine computing started");

    int px, py;

    // result index to store in
    int ri;

    // current iter
    int ci;

    // starting imag
    double start_c_i = fr->prop.center_y + fr->dim.height / (fr->dim.width * fr->prop.zoom);

    // complex numbers, and their components squared
    double z_r, z_i, z_r2, z_i2;
    double c_r = fr->prop.center_x - 1.0 / fr->prop.zoom, 
           c_i = start_c_i;

    // delta per pixel, this is subtracted from Y, but added to X
    double delta_ppx = 2.0 / (fr->dim.width * fr->prop.zoom);


    // temporary variable
    double tmp;

    for (px = 0; px < fr->dim.width; ++px) {
        c_i = start_c_i;
        for (py = 0; py < fr->dim.height; ++py) {
            ri = 4 * (py * fr->dim.width + px);
            z_r = c_r;
            z_i = c_i;
            z_r2 = z_r * z_r;
            z_i2 = z_i * z_i;

            for (ci = 0; z_r2 + z_i2 <= fr->prop.er2 && ci < fr->prop.max_iter; ++ci) {
                tmp = 2 * z_r * z_i;
                z_r = z_r2 - z_i2 + c_r;
                z_i = tmp + c_i;
                z_r2 = z_r * z_r;
                z_i2 = z_i * z_i;
            }

            fr_col_fillinidx(ci, z_r2 + z_i2, ri, fr);
            
            c_i -= delta_ppx;
        }
        c_r += delta_ppx;
    }
    log_debug("c engine computing ended");
}

