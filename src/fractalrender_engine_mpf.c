/* fractalrender_engine_mpf.h -- GMP mpf engine

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

#include "fractalrender_engine_mpf.h"

e_mpf_fr_t mpf_fr;

fr_engine_lib_export_t fr_engine_mpf_export = {
    fr_engine_mpf_init,
    fr_engine_mpf_compute
};


int prec;

void engine_mpf_print_help() {
    printf("MPF Engine Help\n");
    printf("  -p             precision in bits\n");
    printf("  -h             print this help menu\n");
    printf("\n");
}


void fr_engine_mpf_init(fr_t * fr) {
    log_debug("mpf engine initialized");

    char c;

    prec = 64;

    if (fr->argc > 1) {
        while ((c = getopt (fr->argc, fr->argv, "p:h")) != optstop) {
            switch (c) {
                case 'p':
                    prec = atoi(optarg);
                    break;
                case 'h':
                    engine_mpf_print_help();
                    FR_FINISH
                    break;
                case '?':
                    engine_mpf_print_help();
                    FR_FINISH
                    break;
                default:
                    log_fatal("unknown getopt");
                    break;
            }
        }
    }

    mpf_set_default_prec(prec);

    mpf_init(mpf_fr.cX);
    mpf_init(mpf_fr.cY);
    mpf_init(mpf_fr.Z);
    mpf_init(mpf_fr.c_r);
    mpf_init(mpf_fr.s_c_i);
    mpf_init(mpf_fr.c_i);
    mpf_init(mpf_fr.z_r);
    mpf_init(mpf_fr.z_i);
    mpf_init(mpf_fr.z_r2);
    mpf_init(mpf_fr.z_i2);
    mpf_init(mpf_fr.tmp);
    mpf_init(mpf_fr.delta_ppx);

    log_debug("mpf engine: using precision of %d", prec);
}

void fr_engine_mpf_compute(fr_t * fr) {
    log_debug("mpf engine computing started");

    mpf_set_prec(mpf_fr.cX, fr->prop.prec);
    mpf_set_prec(mpf_fr.cY, fr->prop.prec);
    mpf_set_prec(mpf_fr.Z, fr->prop.prec);
    mpf_set_prec(mpf_fr.c_r, fr->prop.prec);
    mpf_set_prec(mpf_fr.s_c_i, fr->prop.prec);
    mpf_set_prec(mpf_fr.c_i, fr->prop.prec);
    mpf_set_prec(mpf_fr.z_r, fr->prop.prec);
    mpf_set_prec(mpf_fr.z_i, fr->prop.prec);
    mpf_set_prec(mpf_fr.z_r2, fr->prop.prec);
    mpf_set_prec(mpf_fr.z_i2, fr->prop.prec);
    mpf_set_prec(mpf_fr.tmp, fr->prop.prec);
    mpf_set_prec(mpf_fr.delta_ppx, fr->prop.prec);

    int px, py;

    // result index to store in
    int ri;

    // current iter
    int ci;

    mpf_set_str(mpf_fr.cX, fr->prop.center_x_str, 10);
    mpf_set_str(mpf_fr.cY, fr->prop.center_y_str, 10);
    mpf_set_str(mpf_fr.Z, fr->prop.zoom_str, 10);


    mpf_set_ui(mpf_fr.s_c_i, fr->dim.height);
    mpf_div_ui(mpf_fr.s_c_i, mpf_fr.s_c_i, fr->dim.width);
    mpf_div(mpf_fr.s_c_i, mpf_fr.s_c_i, mpf_fr.Z);
    mpf_add(mpf_fr.s_c_i, mpf_fr.cY, mpf_fr.s_c_i);

    mpf_ui_div(mpf_fr.c_r, 1, mpf_fr.Z);
    mpf_sub(mpf_fr.c_r, mpf_fr.cX, mpf_fr.c_r);

    mpf_set_ui(mpf_fr.delta_ppx, 2);
    mpf_div_ui(mpf_fr.delta_ppx, mpf_fr.delta_ppx, fr->dim.width);
    mpf_div(mpf_fr.delta_ppx, mpf_fr.delta_ppx, mpf_fr.Z);


    log_trace("mpf engine: center_x: %s, center_y: %s, zoom: %s", fr->prop.center_x_str, fr->prop.center_y_str, fr->prop.zoom_str);

    for (px = 0; px < fr->dim.width; ++px) {
        mpf_set(mpf_fr.c_i, mpf_fr.s_c_i);
        for (py = 0; py < fr->dim.height; ++py) {
            ri = py * fr->dim.mem_width + fr->dim.byte_depth * px;
            mpf_set(mpf_fr.z_r, mpf_fr.c_r);
            mpf_set(mpf_fr.z_i, mpf_fr.c_i);

            mpf_mul(mpf_fr.z_r2, mpf_fr.z_r, mpf_fr.z_r);
            mpf_mul(mpf_fr.z_i2, mpf_fr.z_i, mpf_fr.z_i);

            mpf_add(mpf_fr.tmp, mpf_fr.z_r2, mpf_fr.z_i2);

            for (ci = 0; mpf_cmp_d(mpf_fr.tmp, fr->prop.er2) <= 0 && ci < fr->prop.max_iter; ++ci) {
                mpf_mul(mpf_fr.tmp, mpf_fr.z_r, mpf_fr.z_i);
                mpf_mul_ui(mpf_fr.tmp, mpf_fr.tmp, 2);
                mpf_sub(mpf_fr.z_r, mpf_fr.z_r2, mpf_fr.z_i2);
                mpf_add(mpf_fr.z_r, mpf_fr.z_r, mpf_fr.c_r);
                mpf_add(mpf_fr.z_i, mpf_fr.tmp, mpf_fr.c_i);
                mpf_mul(mpf_fr.z_r2, mpf_fr.z_r, mpf_fr.z_r);
                mpf_mul(mpf_fr.z_i2, mpf_fr.z_i, mpf_fr.z_i);
                mpf_add(mpf_fr.tmp, mpf_fr.z_r2, mpf_fr.z_i2);
            }

            fr_col_fillinidx(ci, mpf_get_d(mpf_fr.tmp), ri, fr);

            mpf_sub(mpf_fr.c_i, mpf_fr.c_i, mpf_fr.delta_ppx);
        }
        mpf_add(mpf_fr.c_r, mpf_fr.c_r, mpf_fr.delta_ppx);
    }
    log_debug("mpf engine computing ended");
}
