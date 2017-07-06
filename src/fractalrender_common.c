/* fractalrender_common.h -- defines types and common methods

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


char * fr_get_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if  (!dot || dot == filename) return "";
    return dot + 1;
}

double fr_diffsec(struct timeval a, struct timeval b) {
    return (a.tv_sec - b.tv_sec) + (a.tv_usec - b.tv_usec) / 1.0e6;
}

//
void fr_ctime(struct timeval *ret) {
    #ifdef _WIN32
    log_error("Haven't implemented for windows yet");
    #else
    gettimeofday(ret, NULL);
    #endif
}


void fr_init(fr_t * fr, fr_dim_t dim) {
    assert(dim.width > 0 && dim.height > 0);
    fr->dim = dim;

    fr->dim.byte_depth = FR_BYTEDEPTH;

    fr->dim.mem_width = fr->dim.width * fr->dim.byte_depth;

    fr->bitmap = (unsigned char *)malloc(fr->dim.mem_width * fr->dim.height);
    assert(fr->bitmap != NULL);

    // start in color at NULL
    fr->col.in_col = NULL;

}

int strcicmp(char *a, char *b) {
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a) return d;
    }
}

void fr_clear(fr_t * fr) {
    if (fr->bitmap != NULL) {
        free(fr->bitmap);
    }
    if (fr->col.in_col != NULL) {
        free(fr->col.in_col);
    }
}

void fr_set_col(fr_t * fr, fr_col_t col, char * color_scheme) {
    assert(col.col_len > 1);
    assert(col.scale != 0.0);
    fr->col = col;

    int i;

    // if they give us NULL, then we need to try and allocate enough and use FR_COL_* to determine what these need
    if (fr->col.in_col == NULL && color_scheme != NULL) {
        if (STR_EQ(fr_get_ext(color_scheme), "color")) {
            log_info("reading color scheme from file: %s", color_scheme);
            // read from .color file
            FILE * fp = fr_sfopen(color_scheme, "r");
            int R, G, B, A, ct;
            for (ct = 0; fscanf(fp, "%d,%d,%d,%d\n", &R, &G, &B, &A) == 4; ++ct) {
                if (!FR_VALID_RGBA(R, G, B, A)) {
                    fclose(fp);
                    log_error("error reading color file: %s", color_scheme);
                    FR_FAIL
                }
            }
            fclose(fp);
            fr->col.col_len = ct;
            fr->col.in_col = (unsigned char *)malloc(4 * fr->col.col_len);
            fp = fr_sfopen(color_scheme, "r");


            for (i = 0; i < ct; ++i) {
                assert(fscanf(fp, "%d,%d,%d,%d\n", &R, &G, &B, &A) == 4);
                fr->col.in_col[4 * i + 0] = R;
                fr->col.in_col[4 * i + 1] = G;
                fr->col.in_col[4 * i + 2] = B;
                fr->col.in_col[4 * i + 3] = A;
            }

        } else {
            log_info("using color scheme builtin: %s", color_scheme);

            // index, what to set result to
            void (*color_gen_func)(int,fr_col_t *) = NULL;

            if (STR_EQIC(color_scheme, "red")) {
                color_gen_func = &fr_col_gen_red;
            } else if (STR_EQIC(color_scheme, "green")) {
                color_gen_func = &fr_col_gen_green;
            } else if (STR_EQIC(color_scheme, "blue")) {
                color_gen_func = &fr_col_gen_blue;
            } else if (STR_EQIC(color_scheme, "mocha")) {
                  color_gen_func = &fr_col_gen_mocha;
            } else if (STR_EQIC(color_scheme, "random")) {
                color_gen_func = &fr_col_gen_random;
            } else {
                log_error("Unknown color scheme: %s", color_scheme);
            }

            assert(color_gen_func != NULL);
            fr->col.in_col = (unsigned char *)malloc(fr->dim.byte_depth * fr->col.col_len);
            assert(fr->col.in_col != NULL);
            int ci;
            for (ci = 0; ci < fr->col.col_len; ++ci) {
                (*color_gen_func)(ci, &fr->col);
            }
        }

        log_trace("printing out color scheme used:");

        if (log_get_level() >= LOG_TRACE) {
            unsigned char * tmp_col;

            for (i = 0; i < fr->col.col_len; ++i) {
                tmp_col = fr->col.in_col + fr->dim.byte_depth * i;
                printf("%d,%d,%d,%d\n", tmp_col[0], tmp_col[1], tmp_col[2], tmp_col[3]);
            }
        }

    } else {
        log_warn("fr_set_col(): not doing anything, because given NULL values");
        // do nothing, either we were given a buf, or they specified no color
    }

}


FILE * fr_sfopen(char * fname, char * mode) {
    FILE * fp_return = fopen(fname, mode);
    if (fp_return == NULL) {
        log_fatal("Failed opening file '%s' with mode %s", fname, mode);
    }

    return fp_return;
}
