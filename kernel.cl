__kernel void mand(__global __const int *imgMeta, __global __const double * meta, __global int * data)
{
    int width = imgMeta[0];
    int height = imgMeta[1];
    int maxIter = imgMeta[2];
    
    int px = get_global_id(0);
    int py = get_global_id(1);

    double cx = meta[0];
    double cy = meta[1];
    double zoom = meta[2];
    double x = cx + (2 * px - width) / (zoom * width);
    double y = cy + (height - 2 * py) / (zoom * width);

    double sx = x;
    double sy = y;
    double xs = x * x;
    double ys = y * y;
    double tmp;
    
    int iter = 0;
    while (xs + ys <= 4) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        iter += 1;
        if (iter >= maxIter) break;
        xs = x * x;
        ys = y * y;
    }
    data[py * width + px] = iter;
}
