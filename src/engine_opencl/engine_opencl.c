/* engine_opencl/engine_opencl.c -- Engine that uses OpenCL kernel

  Copyright 2016-2017 ChemicalDevelopment

  This file is part of the FractalRender project.

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


#include "fractalrender.h"

/*

imgMeta is width, height, max_iter

meta is cX, cY, and Z

This is a quick abstraction so that the kernel can get this info

*/

#define ENGINE_OPENCL_KERNEL_SOURCE_DEFAULT "\
__kernel void mand_64in_16out(__global __const int *imgMeta, __global __const double * meta, __global ushort * data)\
{\
    int px = get_global_id(0), py = get_global_id(1);\
\
    double x = meta[0] + (2 * px - imgMeta[0]) / (meta[2] * imgMeta[0]), y = meta[1] + (imgMeta[1] - 2 * py) / (meta[2] * imgMeta[0]);\
\
    double sx = x, sy = y, xs = x * x, ys = y * y, tmp;\
    \
    ushort iter = 0;\
    while (xs + ys <= 4) {\
        tmp = 2 * x * y;\
        x = xs - ys + sx;\
        y = tmp + sy;\
        iter += 1;\
        if (iter >= imgMeta[2] - 1) break;\
        xs = x * x;\
        ys = y * y;\
    }\
    data[py * imgMeta[0] + px] = iter;\
}\
\
__kernel void mand_64in_32out(__global __const int *imgMeta, __global __const double * meta, __global uint * data)\
{\
    int px = get_global_id(0), py = get_global_id(1);\
\
    double x = meta[0] + (2 * px - imgMeta[0]) / (meta[2] * imgMeta[0]), y = meta[1] + (imgMeta[1] - 2 * py) / (meta[2] * imgMeta[0]);\
\
    double sx = x, sy = y, xs = x * x, ys = y * y, tmp;\
    \
    uint iter = 0;\
    while (xs + ys <= 4) {\
        tmp = 2 * x * y;\
        x = xs - ys + sx;\
        y = tmp + sy;\
        iter += 1;\
        if (iter >= imgMeta[2] - 1) break;\
        xs = x * x;\
        ys = y * y;\
    }\
    data[py * imgMeta[0] + px] = iter;\
}\
\
"



#define MAX_SOURCE_SIZE (0x100000)



cl_device_id device_id = NULL;
cl_context context = NULL;
cl_command_queue command_queue = NULL;
cl_program program = NULL;
cl_kernel kernel = NULL;
cl_platform_id platform_id = NULL;
cl_uint res_num_devices;
cl_uint res_num_platforms;
cl_int res;

cl_mem imgMeta_buf = NULL;
cl_mem meta_buf = NULL;
cl_mem data_buf = NULL;



size_t *global_item_size;
size_t *local_item_size;

void engine_opencl_error_handle(char *file, int line, char *src, int code) {
    if (code) {
        printf("OpenCL error code: %d\n", code);
        printf("%s:%d\n", file, line);
        printf("%s\n", src);
        exit(3);
    } else {
        // do nothing, no error
    }
}

#define CLGLBL_HNDL(ST) ST; engine_opencl_error_handle(__FILE__, __LINE__, #ST, res);


void engine_opencl_init(int __depth, int d0, int d1) {

    char *source_str;
    size_t source_size;

    if (cargs_get_flag("-CLkernel")) {
        FILE *fp = fopen(cargs_get("-CLkernel"), "r");
        source_str = (char*)malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);
    } else {
        source_str = ENGINE_OPENCL_KERNEL_SOURCE_DEFAULT;
        source_size = strlen(source_str);
    }


    CLGLBL_HNDL(res = clGetPlatformIDs(1, &platform_id, &res_num_platforms));

    // select which device here
    //printf("platformid: %d\n", platform_id);
    CLGLBL_HNDL(res = clGetDeviceIDs(0, CL_DEVICE_TYPE_GPU, 1, &device_id, &res_num_devices));


    CLGLBL_HNDL(context = clCreateContext(0, 1, &device_id, NULL, NULL, &res));

    

    CLGLBL_HNDL(command_queue = clCreateCommandQueue(context, device_id, 0, &res));

    CLGLBL_HNDL(program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &res));

    CLGLBL_HNDL(res = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL));

    // switch based on kernel
    char *kernel_name;
    if (__depth == 16) {
        kernel_name = "mand_64in_16out";
    } else if (__depth == 32) {
        kernel_name = "mand_64in_32out";
    } else {
        printf("Unsupported OpenCL kernel bit depth: %d\n", __depth);
        FR_FAIL
    }

    CLGLBL_HNDL(kernel = clCreateKernel(program, kernel_name, &res));

	global_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	local_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	global_item_size[0] = d0;
	global_item_size[1] = d1;
	local_item_size[0] = cargs_get_int_idx("-CLsize", 0);
	local_item_size[1] = cargs_get_int_idx("-CLsize", 1);

    CLGLBL_HNDL(imgMeta_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * sizeof(int), NULL, &res));

	
    CLGLBL_HNDL(meta_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * sizeof(double), NULL, &res));

    CLGLBL_HNDL(data_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, global_item_size[0] * global_item_size[1] * sizeof(FR_16BIT), NULL, &res));

}


void engine_opencl_end() {
	CLGLBL_HNDL(res = clFlush(command_queue));
	CLGLBL_HNDL(res = clFinish(command_queue));
	CLGLBL_HNDL(res = clReleaseKernel(kernel));
	CLGLBL_HNDL(res = clReleaseProgram(program));
	CLGLBL_HNDL(res = clReleaseMemObject(data_buf));
	CLGLBL_HNDL(res = clReleaseMemObject(meta_buf));
	CLGLBL_HNDL(res = clReleaseMemObject(imgMeta_buf));
	CLGLBL_HNDL(res = clReleaseCommandQueue(command_queue));
	CLGLBL_HNDL(res = clReleaseContext(context));
}


void engine_opencl_fulltest(fractal_img_t * ret) {

    assert(ret->depth == 16);

	int *imgMeta = (int *)malloc(3 * sizeof(int));
    double * meta = (double *)malloc(3 * sizeof(double));

    long long *data = (long long *)ret->data;

    imgMeta[0] = ret->px;
    imgMeta[1] = ret->py;
    imgMeta[2] = ret->max_iter;

    meta[0] = atof(ret->cX);
    meta[1] = atof(ret->cY);
    meta[2] = atof(ret->Z);

	CLGLBL_HNDL(clEnqueueWriteBuffer(command_queue, imgMeta_buf, CL_TRUE, 0, 3 * sizeof(int), imgMeta, 0, NULL, NULL));

	CLGLBL_HNDL(clEnqueueWriteBuffer(command_queue, meta_buf, CL_TRUE, 0, 3 * sizeof(double), meta, 0, NULL, NULL));

	//clEnqueueWriteBuffer(command_queue, data_buf, CL_TRUE, 0, ret->px * ret->py * sizeof(FR_16BIT), data, 0, NULL, NULL);

    CLGLBL_HNDL(res = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imgMeta_buf));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&meta_buf));
	CLGLBL_HNDL(res = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&data_buf));

    CLGLBL_HNDL(res = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL));

    CLGLBL_HNDL(res = clEnqueueReadBuffer(command_queue, data_buf, CL_TRUE, 0, ret->px * ret->py * sizeof(FR_16BIT), ret->data, 0, NULL, NULL));

}



