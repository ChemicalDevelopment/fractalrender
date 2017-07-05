/* fractalrender_types.h -- defines types and common methods

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

#ifndef __FRACTALRENDER_TYPES_H__
#define __FRACTALRENDER_TYPES_H__


// dimensions
typedef struct fr_dim_t {
    unsigned long width, height;

    // should be normally width..., but SDL requires this
    unsigned long mem_width, byte_depth;
} fr_dim_t;


// common colorizer
typedef struct fr_col_t {

    unsigned long col_len;

    double offset, scale;

    bool is_simple;

    // R, G, B, A, 4 * col_len
    unsigned char * in_col;

} fr_col_t;


// properties, speicifically math properties
typedef struct fr_prop_t {
    // store string representations of some properties, so that multi-precision libraries can read them in.


    char * center_x_str, * center_y_str;
    double center_x, center_y;

    char * base_zoom_str, * zoom_str;
    double base_zoom, zoom;

    unsigned int max_iter;
    int prec;


    char *er_str;

    double er, er2;

} fr_prop_t;


// for storing output
typedef struct fr_out_t {

    char * file_tmp_out;
    char * file_tmp_out_ext;

    char * file_out;
    char * file_out_ext;

    char * file_write_out;

} fr_out_t;

// animation type
typedef struct fr_anim_t {
    bool do_anim;

    char *zoomps_str;
    double zoomps, sec;

    int fps;

    int total_frames;

    int c_frame;
    double c_time;

} fr_anim_t;



// fractal type
typedef struct fr_t {

    fr_dim_t dim;

    fr_col_t col;

    fr_prop_t prop;

    fr_anim_t anim;


    int argc;

    char ** argv;


    // R, G, B, A, 4 * dim.width * dim.height
    unsigned char * bitmap;

} fr_t;

#endif
