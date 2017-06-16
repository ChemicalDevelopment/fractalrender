
__kernel void mand(__global __const int *imgMeta, __global __const double * meta, __global uchar * incol, __global uchar * outcol)
{
    int px = get_global_id(0), py = get_global_id(1);

    double x = meta[0] + (2 * px - imgMeta[0]) / (meta[2] * imgMeta[0]), y = meta[1] + (imgMeta[1] - 2 * py) / (meta[2] * imgMeta[0]);

    double sx = x, sy = y, xs = x * x, ys = y * y, tmp;
    
    double hue, di, zn;

    double er = 4.0;
    double er2 = er * er;

    int iter = 1;
    while (xs + ys <= er2) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        iter += 1;
        if (iter >= imgMeta[2]) break;
        xs = x * x;
        ys = y * y;
    }

    zn = xs + ys;
    di = (double) iter;

    if (zn <= er2) {
        hue = 0;
    } else {
        hue = di + 1 - log(fabs(zn)) / log(er2);
    }
    while (hue < 0) {
        hue += imgMeta[3];
    }
    while (hue >= imgMeta[3]) {
        hue -= imgMeta[3];
    }

    int sci0 = (int)floor(hue);
    tmp = hue - sci0;

    int sci1 = sci0 * 3;
    int sci2;

    if (sci0 >= imgMeta[3] - 1) {
        sci2 = 0;
    } else {
        sci2 = sci1 + 3;
    }

    int sci3 = 3 * (py * imgMeta[0] + px);
    outcol[sci3 + 0] = (uchar)(floor(tmp * incol[sci2 + 0] + (1 - tmp) * incol[sci1 + 0]));
    outcol[sci3 + 1] = (uchar)(floor(tmp * incol[sci2 + 1] + (1 - tmp) * incol[sci1 + 1]));
    outcol[sci3 + 2] = (uchar)(floor(tmp * incol[sci2 + 2] + (1 - tmp) * incol[sci1 + 2]));

}
