/* fractalrender_prop_mpf.c -- multiprecision float property handler

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

#include "fractalrender_prop_mpf.h"

fr_prop_lib_export_t fr_prop_mpf_export = {
    fr_prop_init,
    fr_set_prop,
    fr_get_prop,
    fr_set_prec,
    fr_zoomin,
    fr_movex,
    fr_movey
};

mpf_t tmp_mpf[5];


int mpf_prec;
int mpf_dig;

void fr_prop_init(fr_t * fr) {
    log_debug("mpf prop init");
    fr_set_prec(fr, fr->prop.prec);
    mpf_init2(tmp_mpf[0], fr->prop.prec);
    mpf_init2(tmp_mpf[1], fr->prop.prec);
    mpf_init2(tmp_mpf[2], fr->prop.prec);
    mpf_init2(tmp_mpf[3], fr->prop.prec);
    mpf_init2(tmp_mpf[4], fr->prop.prec);
    fr->prop.center_x_str = NULL;
    fr->prop.center_y_str = NULL;
    fr->prop.zoom_str = NULL;
    fr->prop.er_str = NULL;
    fr->prop.base_zoom_str = NULL;
    fr->anim.zoomps_str = NULL;


}

void fr_zoomin(fr_t * fr, double scale) {
    mpf_set_str(tmp_mpf[0], fr->prop.zoom_str, 10);
    mpf_set_d(tmp_mpf[1], scale);
    mpf_mul(tmp_mpf[0], tmp_mpf[0], tmp_mpf[1]);
    char *to_ret = (char *)malloc(mpf_dig + 100);
    gmp_sprintf(to_ret, "%.*Ff", mpf_dig + 10, tmp_mpf[0]);

    fr_set_prop(fr, "zoom", to_ret, 0);
}

void fr_movex(fr_t * fr, double scale) {
    mpf_set_str(tmp_mpf[0], fr->prop.center_x_str, 10);
    mpf_set_str(tmp_mpf[1], fr->prop.zoom_str, 10);
    mpf_set_d(tmp_mpf[2], scale);
    mpf_div(tmp_mpf[2], tmp_mpf[2], tmp_mpf[1]);
    mpf_add(tmp_mpf[0], tmp_mpf[0], tmp_mpf[2]);

    printf("%d\n", mpf_dig);
    char *to_ret = (char *)malloc(mpf_dig + 100);
    gmp_sprintf(to_ret, "%.*Ff", mpf_dig + 10, tmp_mpf[0]);

    fr_set_prop(fr, "center_x", to_ret, 0);
}

void fr_movey(fr_t * fr, double scale) {
    mpf_set_str(tmp_mpf[0], fr->prop.center_y_str, 10);
    mpf_set_str(tmp_mpf[1], fr->prop.zoom_str, 10);
    mpf_set_d(tmp_mpf[2], scale);
    mpf_div(tmp_mpf[2], tmp_mpf[2], tmp_mpf[1]);
    mpf_add(tmp_mpf[0], tmp_mpf[0], tmp_mpf[2]);

    printf("%d\n", mpf_dig);
    char *to_ret = (char *)malloc(mpf_dig + 100);
    gmp_sprintf(to_ret, "%.*Ff", mpf_dig, tmp_mpf[0]);

    fr_set_prop(fr, "center_y", to_ret, 0);
}

void fr_set_prec(fr_t * fr, int prec) {
    mpf_prec = prec;
    mpf_dig = (int)floor(prec * log(2.0) / log(10.0));
    log_debug("mpf prop setting prec to %d, and dig to %d", mpf_prec, mpf_dig);
    mpf_set_default_prec(mpf_prec);
}

// sets to dval if val is null
void fr_set_prop(fr_t * fr, char * name, char * val, double dval) {
  log_trace("setting property '%s' to '%s' (dval: %lf)", name, val, dval);

    char ** target = NULL;
    double * target_d = NULL;
    if (STR_EQIC(name, "base_zoom")) {
        target = &fr->prop.base_zoom_str;
        target_d = &fr->prop.base_zoom;
    } else if (STR_EQIC(name, "zoomps")) {
        target = &fr->anim.zoomps_str;
        target_d = &fr->anim.zoomps;
    } else if (STR_EQIC(name, "center_x")) {
        target = &fr->prop.center_x_str;
        target_d = &fr->prop.center_x;
    } else if (STR_EQIC(name, "center_y")) {
        target = &fr->prop.center_y_str;
        target_d = &fr->prop.center_y;
    } else if (STR_EQIC(name, "zoom")) {
        target = &fr->prop.zoom_str;
        target_d = &fr->prop.zoom;
    } else if (STR_EQIC(name, "er")) {
        target = &fr->prop.er_str;
        target_d = &fr->prop.er;
    } else {
        log_trace("unrecognized name: '%s'", name);
    }

    if (target == NULL || target_d == NULL) {
        log_fatal("fr_set_prop(): unsupported prop name '%s'", name);
    }

    if (val == NULL) {
        if (*target == NULL || strlen(*target) < 30) {
            *target = (char *)malloc(40);
        }
        sprintf(*target, "%30lf", dval);
        *target_d = dval;
    } else {
        if (*target == NULL || strlen(val) > strlen(*target)) {
            *target = (char *)malloc(strlen(val) + 1);
        }
        sprintf(*target, "%s", val);
        *target_d = atof(val);
    }

    fr->prop.er2 = fr->prop.er * fr->prop.er;
}

void fr_get_prop(char **_target, double *_target_d, fr_t * fr, char * name) {
    char * target = NULL;
    double * target_d = NULL;
    if (STR_EQIC(name, "base_zoom")) {
        target = fr->prop.base_zoom_str;
        target_d = &fr->prop.base_zoom;
    } else if (STR_EQIC(name, "zoomps")) {
        target = fr->anim.zoomps_str;
        target_d = &fr->anim.zoomps;
    }

    if (target == NULL && target_d == NULL) {
        log_error("fr_get_prop(): unsupported prop name '%s'", name);
        return;
    }
    if (target != NULL) {
        if (strlen(target) > strlen(*_target)) {
            *_target = (char *)malloc(strlen(target) + 1);
        }
        sprintf(*_target, "%s", target);
    }

    if (target_d != NULL) {
        *_target_d = *target_d;
    }
}
