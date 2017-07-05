/* fractalrender_prop_c.c -- the C implementation of the properties handler

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

#include "fractalrender_prop_c.h"

fr_prop_lib_export_t fr_prop_c_export = {
    fr_prop_init,
    fr_set_prop,
    fr_get_prop,
    fr_set_prec,
    fr_zoomin,
    fr_movex,
    fr_movey
};

void fr_prop_init(fr_t * fr) {
    log_debug("c prop init");
    fr->prop.center_x_str = NULL;
    fr->prop.center_y_str = NULL;
    fr->prop.zoom_str = NULL;
    fr->prop.er_str = NULL;
    fr->prop.base_zoom_str = NULL;
    fr->anim.zoomps_str = NULL;

}

void fr_zoomin(fr_t * fr, double scale) {
    fr_set_prop(fr, "zoom", NULL, fr->prop.zoom * scale);
}

void fr_movex(fr_t * fr, double scale) {
    fr_set_prop(fr, "center_x", NULL, fr->prop.center_x + scale / fr->prop.zoom);
}

void fr_movey(fr_t * fr, double scale) {
    fr_set_prop(fr, "center_y", NULL, fr->prop.center_y + scale / fr->prop.zoom);
}

void fr_set_prec(fr_t * fr, int prec) {
    log_debug("fr_set_prec() using prop_c called with prec: %d", prec);
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
