/* fractalrender.h -- header file for fractalrender.c

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

#ifndef __FRACTALRENDER_DEFS_H__
#define __FRACTALRENDER_DEFS_H__

#ifdef HAVE_GMP
typedef struct fractal_mpf_t {

    mpf_t cX, cY, Z;

    mpf_t d_c;

    mpf_t ssp_x, ssp_y;

    mpf_t sp_x, sp_y;

    mpf_t p_x, p_y;
    mpf_t p_x_s, p_y_s;

    mpf_t tmp;

} fractal_mpf_t;
#endif


// this keeps track of colors
typedef struct fractal_color_t {

    long numcol;

    // in form FR_COLOR_$X
    long coltype;

    int is_simple;

    double mult, disp;

    // 3 * numcol, in RGB format
    unsigned char * data;

} fractal_color_t;

// mapping object
typedef struct fractal_img_t {
    long px, py, max_iter;

    // FR_COLOR_X, in tofile.h
    long imgfmt;

    double num_pixels_total;

    double ctime;

    bool is_anim;

    // generic output format, output file
    char *tmpout, * out;

    fractal_color_t color;

    long prec;

    char *engine;


    long depth;

    char *cX, *cY;

    char *Z;

    // r, g, b
    unsigned char * data;

} fractal_img_t;



#endif
