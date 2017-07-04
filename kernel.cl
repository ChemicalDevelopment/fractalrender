

void colorfunc(__global uchar * color_pattern, int color_pattern_length, int color_pattern_scale, int color_pattern_disp, __global uchar * color_output, float zn, int ci, int max_iter, float er2, int col_dest, int do_simple_color);


void colorfunc(__global uchar * color_pattern, int color_pattern_length, int color_pattern_scale, int color_pattern_disp, __global uchar * color_output, float zn, int ci, int max_iter, float er2, int col_dest, int do_simple_color) {

    if (do_simple_color) {
        int color_off;

        if (ci >= max_iter) {
            color_off = 0;
        } else {
            color_off = 3*(ci * color_pattern_scale + color_pattern_disp) % color_pattern_length;
        }

        color_output[col_dest + 0] = color_pattern[color_off + 0];
        color_output[col_dest + 1] = color_pattern[color_off + 1];
        color_output[col_dest + 2] = color_pattern[color_off + 2];
    } else {
        float tmp;
        float hue;
        if (zn <= er2) {
            hue = 0;
        } else {
            hue = ci + 1.0f - log(fabs(zn)) / log(er2);
        }

        hue = hue * color_pattern_scale + color_pattern_disp;

        hue = fmod(fmod(hue, color_pattern_length) + color_pattern_length, color_pattern_length);

        tmp = hue - floor(hue);
        int color_off0 = 3 * ((int)floor(hue) % color_pattern_length);
        int color_off1;
        if (color_off0 >= 3 *(color_pattern_length - 1)) {
            color_off1 = 0;
        } else {
            color_off1 = color_off0 + 3;
        }

        color_output[col_dest + 0] = ((uchar)floor(tmp*color_pattern[color_off1 + 0]+(1-tmp)*color_pattern[color_off0 + 0]));
        color_output[col_dest + 1] = ((uchar)floor(tmp*color_pattern[color_off1 + 1]+(1-tmp)*color_pattern[color_off0 + 1]));
        color_output[col_dest + 2] = ((uchar)floor(tmp*color_pattern[color_off1 + 2]+(1-tmp)*color_pattern[color_off0 + 2]));
    }
}


#ifdef cl_khr_fp64


__kernel void mand(__global uchar * color_pattern, int color_pattern_length, double color_pattern_scale, double color_pattern_disp, int do_simple_color, __global uchar * color_output, int width, int height, int max_iter, double center_x, double center_y, double zoom, double time)
{
    int px = get_global_id(0), py = get_global_id(1);

    double x = center_x + (2 * px - width) / (zoom * width), y = center_y + (height - 2 * py) / (zoom * width);

    double sx = x, sy = y, xs = x * x, ys = y * y, tmp;

    double er = 16.0;
    double er2 = er * er;

    int col_dest = 3 * (py * width + px);

    int ci;
    for (ci = 0; ci < max_iter && xs + ys <= er2; ++ci) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        xs = x * x;
        ys = y * y;
    }

    colorfunc(color_pattern, color_pattern_length, color_pattern_scale, color_pattern_disp, color_output, (float)(xs + ys), ci, max_iter, (float)er2, col_dest, do_simple_color);

}

#endif

__kernel void mand_32(__global uchar * color_pattern, int color_pattern_length, float color_pattern_scale, float color_pattern_disp, int do_simple_color, __global uchar * color_output, int width, int height, int max_iter, float center_x, float center_y, float zoom, float time)
{
    int px = get_global_id(0), py = get_global_id(1);

    float x = center_x + (2 * px - width) / (zoom * width), y = center_y + (height - 2 * py) / (zoom * width);

    float sx = x, sy = y, xs = x * x, ys = y * y, tmp;

    float er = 16.0f;
    float er2 = er * er;

    int col_dest = 3 * (py * width + px);

    int ci;
    for (ci = 0; ci < max_iter && xs + ys <= er2; ++ci) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        xs = x * x;
        ys = y * y;
    }

    colorfunc(color_pattern, color_pattern_length, color_pattern_scale, color_pattern_disp, color_output, xs + ys, ci, max_iter, er2, col_dest, do_simple_color);


}
