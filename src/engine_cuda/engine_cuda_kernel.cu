extern "C" {
  #include <stdio.h>
  #include <stdlib.h>

  __global__
  void mand_cuda_internal(unsigned char * color_output, int width, int height, int max_iter, double center_x, double center_y, double zoom) {
    int px = blockIdx.x, py = blockIdx.y;
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

    color_output[col_dest] = (3 * ci) & 0xff;
  }

  void mand_cuda(unsigned char ** color_output, int width, int height, int max_iter, double center_x, double center_y, double zoom) {
    unsigned char * _gpu_outcol;
    cudaMalloc((void **)&_gpu_outcol, width * height * 3);

    dim3 dimBlock(1,1,1);
    dim3 dimGrid(width,height,1);

    mand_cuda_internal<<<dimGrid,dimBlock>>>(_gpu_outcol, width, height, max_iter, center_x, center_y, zoom);

    cudaMemcpy(*color_output, _gpu_outcol, width * height * 3, cudaMemcpyDeviceToHost);


    cudaFree(_gpu_outcol);


  }
}
