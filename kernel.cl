


#pragma cl_khr_fp64

void colorfunc(int ci, double zn2, double er2, int ri, int is_simple, double col_scale, double col_offset, int byte_depth, int col_len, __global uchar * in_col, __global uchar * bitmap) {
    if (is_simple) {
        int colci;
        if (zn2 < er2) {
            colci = 0;
        } else {
            colci = (int)floor(ci * col_scale + col_offset);
            colci = byte_depth * (colci % col_len);
        }
        bitmap[ri + 0] = in_col[colci + 0];
        bitmap[ri + 1] = in_col[colci + 1];
        bitmap[ri + 2] = in_col[colci + 2];
        bitmap[ri + 3] = in_col[colci + 3];
    } else {
        // fractional index
        double fri, mixfactor;
        if (zn2 < er2) {
            // index = 0, because it is inside the set
            fri = 0;
            mixfactor = 0;
            bitmap[ri + 0] = in_col[0];
            bitmap[ri + 1] = in_col[1];
            bitmap[ri + 2] = in_col[2];
            bitmap[ri + 3] = in_col[3];
        } else {
            fri = 1 + ci - log(log(sqrt(zn2))) / log(2.0);
            fri = fri * col_scale + col_offset;
            fri = fmod(fmod(fri, col_len) + col_len, col_len);

            mixfactor = fri - floor(fri);
            int colci0, colci1;
            
            colci0 = (int)floor(fri);

            if (colci0 >= col_len - 1) {
                colci1 = 0;
            } else {
                colci1 = colci0 + 1;
            }

            colci0 *= byte_depth;
            colci1 *= byte_depth;


            bitmap[ri + 0] = (int)floor(mixfactor * in_col[colci1 + 0] + (1 - mixfactor) * in_col[colci0 + 0]);
            bitmap[ri + 1] = (int)floor(mixfactor * in_col[colci1 + 1] + (1 - mixfactor) * in_col[colci0 + 1]);
            bitmap[ri + 2] = (int)floor(mixfactor * in_col[colci1 + 2] + (1 - mixfactor) * in_col[colci0 + 2]);
            bitmap[ri + 3] = (int)floor(mixfactor * in_col[colci1 + 3] + (1 - mixfactor) * in_col[colci0 + 3]);
            if (bitmap[ri + 3] == 0) {
                bitmap[ri + 3] = 255;
            }

        }
    }
}



__kernel void mand(double center_x, double center_y, double zoom, int height, int width, int mem_width, double er2, int max_iter, int is_simple, double col_scale, double col_offset, int byte_depth, int col_len, __global uchar * in_col, __global uchar * bitmap)
{
    int px = get_global_id(0), py = get_global_id(1);

    double x = center_x + (2 * px - width) / (zoom * width), y = center_y + (height - 2 * py) / (zoom * width);

    double sx = x, sy = y, xs = x * x, ys = y * y, tmp;

    int col_dest = py * mem_width + px * byte_depth;

    int ci;
    for (ci = 0; ci < max_iter && xs + ys <= er2; ++ci) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        xs = x * x;
        ys = y * y;
    }

    colorfunc(ci, xs + ys, er2, col_dest, is_simple, col_scale, col_offset, byte_depth, col_len, in_col, bitmap);

}
