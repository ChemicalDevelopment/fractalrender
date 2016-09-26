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
    double x = cx - 1.0 / zoom + 2 * (px + 0.0) / (zoom * width);
    double y = cy + 1.0 / zoom - 2 * (py + 0.0) / (zoom * height);

    double sx = x;
    double sy = y;
    double tmp;
    
    int iter = 0;
    while (x * x + y * y <= 4) {
        tmp = 2 * x * y;
        x = x * x - y * y + sx;
        y = tmp + sy;
        iter += 1;
        if (iter >= maxIter) break; 
    }
    data[py * width + px] = iter;
}