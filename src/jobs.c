/* jobs.c --

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

void figure_out_job(fractal_img_t *fractal) {
    bool is_anim = fractal->is_anim;
    bool is_mpf = strcmp(cargs_get("-e"), "MPF") == 0;
    if (cargs_get_flag("--no-image")) {
        if (cargs_get_flag("")) {
            printf("Not creating file: %s, because `--no-image` was set\n", cargs_get(""));
        }
        exit(0);
    }

    if (is_anim) {
        if (is_mpf) {
            #ifdef HAVE_GMP
            do_anim_mpf(fractal);
            #endif
        } else {
            do_anim(fractal);
        }
    } else {
        if (is_mpf) {
            #ifdef HAVE_GMP
            do_single_mpf(fractal);
            #endif
        } else {
            do_single(fractal);
        }
    }
}

#ifdef HAVE_GMP

mpf_t frtmp[4];

void __mpf_nroot_fp(mpf_t r, mpf_t x, mpf_t a, int n) {
    mpf_pow_ui(r, x, n - 1);
    mpf_mul_ui(r, r, n);

}
void __mpf_nroot_f(mpf_t r, mpf_t x, mpf_t a, int n) {
    mpf_pow_ui(frtmp[3], x, n);
    mpf_sub(r, frtmp[3], a);   
}

// r ^ n = a
void __mpf_nroot(mpf_t r, mpf_t a, int n, int prec) {
    // frrtmp[0] is our current guess
    mpf_div_ui(frtmp[0], a, n);
    int citer = 0;
    int cprec = 1;

    while (cprec < prec) {
        __mpf_nroot_f(frtmp[1], frtmp[0], a, n);
        __mpf_nroot_fp(frtmp[2], frtmp[0], a, n);

        mpf_div(frtmp[1], frtmp[1], frtmp[2]);

        mpf_sub(frtmp[0], frtmp[0], frtmp[1]);

        if (citer > 40) {
            cprec *= 2;
        }
        citer++;
    }

    mpf_set(r, frtmp[0]);

}

int __gcd(int a, int b) {
    int c;
    while ( a != 0 ) {
        c = a; 
        a = b % a;
        b = c;
    }
    return b;
}

// r = a ^ ( p / q )
void __mpf_pow_ui_div_ui(mpf_t r, mpf_t a, int p, int q, int prec) {
    int gg = __gcd(p, q);
    int gp = p / gg, gq = q / gg;

    mpf_pow_ui(r, a, p);
    __mpf_nroot(r, r, q, prec);

}

void do_anim_mpf(fractal_img_t *fractal) {
    fractal_mpf_t mp;
    engine_mpf_init_mpf(fractal, &mp);
    mpf_t basezoom, zps;
    mpf_init2(frtmp[0], fractal->prec+64);
    mpf_init2(frtmp[1], fractal->prec+64);
    mpf_init2(frtmp[2], fractal->prec+64);
    mpf_init2(frtmp[3], fractal->prec+64);

    mpf_init2(basezoom, fractal->prec+64);
    mpf_init2(zps, fractal->prec+64);

    mpf_set_str(basezoom, fractal->Z, 10);
    mpf_set_str(zps, cargs_get("--zps"), 10);

    mpf_set_str(mp.cX, fractal->cX, 10);
    mpf_set_str(mp.cY, fractal->cY, 10);
    
    double sec = atof(cargs_get("--sec"));

    int fps = cargs_get_int("--fps");

    char * basefmt = (char *)malloc(strlen(fractal->tmpout));
    sprintf(basefmt, "%s", fractal->tmpout);
    fractal->out = (char *)malloc(strlen(basefmt) + 40);

    int total_frames = (int)floor(sec * fps);
    assert(total_frames > 1);

    int i;
    for (i = mpi_rank + 1; i <= total_frames; i += mpi_numprocs) {
        __mpf_pow_ui_div_ui(mp.Z, zps, i - 1, fps, fractal->prec);
        mpf_mul(mp.Z, mp.Z, basezoom);


        sprintf(fractal->out, basefmt, i);

        engine_mpf_fulltest(fractal, &mp);
        fractal_to_file(fractal);
    }
    engine_mpf_clear_mpf(fractal, &mp);
}

void do_single_mpf(fractal_img_t *fractal) {
    fractal_mpf_t mp;
    engine_mpf_init_mpf(fractal, &mp);

    mpf_set_str(mp.cX, fractal->cX, 10);
    mpf_set_str(mp.cY, fractal->cY, 10);

    mpf_set_str(mp.Z, fractal->Z, 10);

    engine_mpf_fulltest(fractal, &mp);
    fractal_to_file(fractal);

    engine_mpf_clear_mpf(fractal, &mp);
}

#endif





void do_anim(fractal_img_t *fractal) {
    double sec = atof(cargs_get("--sec")), fps = atof(cargs_get("--fps")), zps = atof(cargs_get("--zps"));
    double basezoom = atof(cargs_get("-z"));

    char * basefmt = (char *)malloc(strlen(fractal->tmpout));
    sprintf(basefmt, "%s", fractal->tmpout);
    fractal->out = (char *)malloc(strlen(basefmt) + 40);

    int total_frames = (int)floor(sec * fps);
    int i;
    for (i = mpi_rank + 1; i <= total_frames; i += mpi_numprocs) {
        sprintf(fractal->Z, "%lf", basezoom * pow(zps, (double)(i-1)/fps));
        sprintf(fractal->out, basefmt, i);
        fractal->ctime = (double)(i-1)/fps;
        
        do_engine_test(fractal);
        fractal_to_file(fractal);
    }
}

void do_single(fractal_img_t *fractal)  {
    do_engine_test(fractal);
    fractal_to_file(fractal);
}





