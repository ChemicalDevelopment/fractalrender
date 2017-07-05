
#include "fractalrender_engine_cuda_helper.h"

extern "C" {

#include <stdio.h>
#include <stdlib.h>

#include "fractalrender_types.h"
#include "fractalrender_engine_cuda.h"

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t code, const char *file, int line) {
   if (code != cudaSuccess) {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      exit(code);
   }
}


__global__
void mand_cuda_internal(fr_t fr, int width, int height, unsigned char * color_input, unsigned char * color_output) {
    //int px = blockIdx.x, py = blockIdx.y;
    int px = (blockIdx.x * blockDim.x) + threadIdx.x;
    int py = (blockIdx.y * blockDim.y) + threadIdx.y;

    //printf("%d:%d\n", px, py);

    double x = fr.prop.center_x - (width - 2 * px) / (fr.prop.zoom * fr.dim.width), y = fr.prop.center_y + (height - 2 * py) / (fr.prop.zoom * fr.dim.width);

    double sx = x, sy = y, xs = x * x, ys = y * y, tmp;


    int ri = fr.dim.mem_width * py + fr.dim.byte_depth * px;

    int ci;
    for (ci = 0; ci < fr.prop.max_iter && xs + ys <= fr.prop.er2; ++ci) {
        tmp = 2 * x * y;
        x = xs - ys + sx;
        y = tmp + sy;
        xs = x * x;
        ys = y * y;
    }

    double zn2 = xs + ys;
    if (fr.col.is_simple) {
        int colci;
        if (zn2 < fr.prop.er2) {
            colci = 0;
        } else {
            colci = (int)floor(ci * fr.col.scale + fr.col.offset);
            colci = fr.dim.byte_depth * (colci % fr.col.col_len);
        }
        color_output[ri + 0] = color_input[colci + 0];
        color_output[ri + 1] = color_input[colci + 1];
        color_output[ri + 2] = color_input[colci + 2];
        color_output[ri + 3] = color_input[colci + 3];
    } else {
        // fractional index
        double fri, mixfactor;
        if (zn2 < fr.prop.er2) {
            // index = 0, because it is inside the set
            fri = 0;
            mixfactor = 0;
            color_output[ri + 0] = color_input[0];
            color_output[ri + 1] = color_input[1];
            color_output[ri + 2] = color_input[2];
            color_output[ri + 3] = color_input[3];
        } else {
            fri = 1 + ci - log(log(sqrt(zn2))) / log(2.0);
            fri = fri * fr.col.scale + fr.col.offset;
            fri = fmod(fmod((float)fri, (float)fr.col.col_len) + fr.col.col_len, (float)fr.col.col_len);

            mixfactor = fri - floor(fri);
            int colci0, colci1;

            colci0 = (int)floor(fri);

            if (colci0 >= fr.col.col_len - 1) {
                colci1 = 0;
            } else {
                colci1 = colci0 + 1;
            }

            colci0 *= fr.dim.byte_depth;
            colci1 *= fr.dim.byte_depth;

            color_output[ri + 0] = (int)floor(mixfactor * color_input[colci1 + 0] + (1 - mixfactor) * color_input[colci0 + 0]);
            color_output[ri + 1] = (int)floor(mixfactor * color_input[colci1 + 1] + (1 - mixfactor) * color_input[colci0 + 1]);
            color_output[ri + 2] = (int)floor(mixfactor * color_input[colci1 + 2] + (1 - mixfactor) * color_input[colci0 + 2]);
            color_output[ri + 3] = (int)floor(mixfactor * color_input[colci1 + 3] + (1 - mixfactor) * color_input[colci0 + 3]);
        }
    }
}

void mand_cuda_init(cuda_fr_t * cuda_fr, fr_t * fr) {
    //cuda_fr->fr = fr;

    int devID;
    cudaDeviceProp props;

    // This will pick the best possible CUDA capable device
    devID = 0;

    checkCudaErrors(cudaSetDevice(devID));
    checkCudaErrors(cudaGetDevice(&devID));
    checkCudaErrors(cudaGetDeviceProperties(&props, devID));

    gpuErrchk(cudaMalloc((void **)&cuda_fr->_gpu_in, fr->dim.byte_depth * fr->col.col_len));
    gpuErrchk(cudaMemcpy(cuda_fr->_gpu_in, fr->col.in_col, fr->dim.byte_depth * fr->col.col_len, cudaMemcpyHostToDevice));

    gpuErrchk(cudaMalloc((void **)&cuda_fr->_gpu_out, fr->dim.mem_width * fr->dim.height));
}

void mand_cuda(cuda_fr_t * cuda_fr) {

    dim3 dimBlock(cuda_fr->dimx, cuda_fr->dimy);
    dim3 dimGrid(cuda_fr->fr->dim.width / cuda_fr->dimx, cuda_fr->fr->dim.height / cuda_fr->dimy);

    mand_cuda_internal<<<dimGrid,dimBlock>>>(*cuda_fr->fr, cuda_fr->fr->dim.width, cuda_fr->fr->dim.height, cuda_fr->_gpu_in, cuda_fr->_gpu_out);

    gpuErrchk(cudaDeviceSynchronize());
    gpuErrchk(cudaPeekAtLastError());

    gpuErrchk(cudaMemcpy(cuda_fr->fr->bitmap, cuda_fr->_gpu_out, cuda_fr->fr->dim.mem_width * cuda_fr->fr->dim.height, cudaMemcpyDeviceToHost));

}

// end extern
}
