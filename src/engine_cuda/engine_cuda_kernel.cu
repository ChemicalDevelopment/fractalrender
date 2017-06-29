#include "engine_cuda/engine_cuda_helper.h"


extern "C" {
  #include <stdio.h>
  #include <stdlib.h>

  #include "engine_cuda/engine_cuda_impl.h"
  #include "fractalrender_defs.h"


  #define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }

  inline void gpuAssert(cudaError_t code, const char *file, int line) {
     if (code != cudaSuccess) {
        fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        exit(code);
     }
  }



  __global__
  void mand_cuda_internal(__cuda_gpu_fractal_t params, unsigned char * color_input, unsigned char * color_output) {
    //int px = blockIdx.x, py = blockIdx.y;
    int px = (blockIdx.x * blockDim.x) + threadIdx.x;
    int py = (blockIdx.y * blockDim.y) + threadIdx.y;

    //printf("%d:%d\n", px, py);

    double x = params.cX + (2 * px - params.px) / (params.Z * params.px), y = params.cY + (params.py - 2 * py) / (params.Z * params.px);

    double sx = x, sy = y, xs = x * x, ys = y * y, tmp;

    double er = 16.0;
    double er2 = er * er;

    int col_dest = 3 * (py * params.px + px);

    int ci;
    for (ci = 0; ci < params.max_iter && xs + ys <= er2; ++ci) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        xs = x * x;
        ys = y * y;
    }

    //double tmp;
    if (params.simplecolor) {
        int color_off;
        if (ci >= params.max_iter) {
            color_off = 0;
        } else {
            color_off = 3*((int)floor(ci * params.colm + params.cold) % params.numcol);
        }
        color_output[col_dest + 0] = color_input[color_off + 0];
        color_output[col_dest + 1] = color_input[color_off + 1];
        color_output[col_dest + 2] = color_input[color_off + 2];

    } else {
        double zn = xs + ys;
        double hue;
        if (zn <= er2) {
            hue = 0;
        } else {
            hue = ci + 1.0 - log(fabs(zn)) / log(er2);
        }

        hue = hue * params.colm + params.cold;

        hue = fmod(fmod(hue, (double)params.numcol) + params.numcol, (double)params.numcol);

        tmp = hue - floor(hue);
        int color_off0 = 3 * ((int)floor(hue) % params.numcol);
        int color_off1;
        if (color_off0 >= 3 *(params.numcol - 1)) {
            color_off1 = 0;
        } else {
            color_off1 = color_off0 + 3;
        }

        color_output[col_dest + 0] = ((unsigned char)floor(tmp*color_input[color_off1 + 0]+(1-tmp)*color_input[color_off0 + 0]));
        color_output[col_dest + 1] = ((unsigned char)floor(tmp*color_input[color_off1 + 1]+(1-tmp)*color_input[color_off0 + 1]));
        color_output[col_dest + 2] = ((unsigned char)floor(tmp*color_input[color_off1 + 2]+(1-tmp)*color_input[color_off0 + 2]));
    }

    //if (ci != 0) printf("%d\n", ci);
    //cuda_color(col_dest, ci, er2, xs + ys, params, color_input, color_output);
  }

  void mand_cuda_init(__cuda_fractal_t * ret, fractal_img_t * reti) {
      ret->_gpu_fractal.px = reti->px;
      ret->_gpu_fractal.py = reti->py;

      ret->_gpu_fractal.cold = reti->color.disp;
      ret->_gpu_fractal.colm = reti->color.mult;
      ret->_gpu_fractal.simplecolor = reti->color.is_simple;

      ret->_gpu_fractal.max_iter = reti->max_iter;

      ret->_gpu_fractal.numcol = reti->color.numcol;

      ret->_gpu_fractal.cX = atof(reti->cX);
      ret->_gpu_fractal.cY = atof(reti->cY);
      ret->_gpu_fractal.Z = atof(reti->Z);

      ret->incol = reti->color.data;
      ret->outcol = reti->data;
      // ret->_gpu_outcol;

      int devID;
      cudaDeviceProp props;

      // This will pick the best possible CUDA capable device
      devID = 0;

      //Get GPU information
      checkCudaErrors(cudaSetDevice(devID));
      checkCudaErrors(cudaGetDevice(&devID));
      checkCudaErrors(cudaGetDeviceProperties(&props, devID));

      gpuErrchk(cudaMalloc((void **)&ret->_gpu_incol, reti->color.numcol * 3));
      gpuErrchk(cudaMemcpy(ret->_gpu_incol, reti->color.data, reti->color.numcol * 3, cudaMemcpyHostToDevice));

      gpuErrchk(cudaMalloc((void **)&ret->_gpu_outcol, reti->px * reti->py * 3));

      return;
  }

  void mand_cuda(__cuda_fractal_t * ret) {

    dim3 dimBlock(ret->cuda_size_x, ret->cuda_size_y);
    dim3 dimGrid(ret->_gpu_fractal.px/ret->cuda_size_x, ret->_gpu_fractal.py/ret->cuda_size_y);

    mand_cuda_internal<<<dimGrid,dimBlock>>>(ret->_gpu_fractal, ret->_gpu_incol, ret->_gpu_outcol);

    gpuErrchk(cudaPeekAtLastError());
    gpuErrchk(cudaDeviceSynchronize());

    gpuErrchk(cudaMemcpy(ret->outcol, ret->_gpu_outcol, ret->_gpu_fractal.px * ret->_gpu_fractal.py * 3, cudaMemcpyDeviceToHost));

  }

  void mand_cuda_end(__cuda_fractal_t * ret) {
    if (ret->_gpu_outcol != NULL) cudaFree(ret->_gpu_outcol);
    if (ret->_gpu_incol != NULL) cudaFree(ret->_gpu_incol);
  }
}
