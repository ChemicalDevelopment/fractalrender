__kernel void mand(__global __const int *imgMeta, __global __const double * meta, __global float * data)
{
    int px = get_global_id(0), py = get_global_id(1);

    double x = meta[0] + (2 * px - imgMeta[0]) / (meta[2] * imgMeta[0]), y = meta[1] + (imgMeta[1] - 2 * py) / (meta[2] * imgMeta[0]);

    double sx = x, sy = y, xs = x * x, ys = y *y, tmp;
    
    int iter = 0;
    while (xs + ys <= 4) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        iter += 1;
        if (iter >= imgMeta[2]) break;
        xs = x * x;
        ys = y * y;
    }
    data[py * imgMeta[0] + px] = .6 + .0015625 * (((imgMeta[2] - iter)<<2) & 0xff);
}