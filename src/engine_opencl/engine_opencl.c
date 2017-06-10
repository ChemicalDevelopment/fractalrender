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

This is a quick

*/


#define ENGINE_OPENCL_KERNEL_SOURCE_DEFAULT " \
__kernel void mand_32(__global __const int *imgMeta, __global __const double * meta, __global int * data) \
{ \
    int px = get_global_id(0), py = get_global_id(1); \
 \
    double x = meta[0] + (2 * px - imgMeta[0]) / (meta[2] * imgMeta[0]), y = meta[1] + (imgMeta[1] - 2 * py) / (meta[2] * imgMeta[0]); \
 \
    double sx = x, sy = y, xs = x * x, ys = y *y, tmp; \
 \
    int iter = 0; \
    while (xs + ys <= 4) { \
        tmp = 2 * x * y; \
        x = xs - ys + sx; \
        y = tmp + sy; \
        iter += 1; \
        if (iter >= imgMeta[2] - 1) break; \
        xs = x * x; \
        ys = y * y; \
    } \
    data[py * imgMeta[0] + px] = iter; \
} "


#define MAX_SOURCE_SIZE (0x100000)

void engine_opencl_fulltest(fractal_img_t * res) {

    assert(res->depth == 32);

    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;

    cl_mem imgMeta_buf = NULL;
    cl_mem meta_buf = NULL;
    cl_mem data_buf = NULL;


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


    size_t *global_item_size;
    size_t *local_item_size;

    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &ret);
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "mand_32", &ret);

	global_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	local_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	global_item_size[0] = res->px;
	global_item_size[1] = res->py;
	local_item_size[0] = 16;
	local_item_size[1] = 16;

	int *imgMeta = (int *)malloc(3 * sizeof(int));
    double * meta = (double *)malloc(3 * sizeof(double));

    long long *data = (long long *)res->data;

    imgMeta[0] = res->px;
    imgMeta[1] = res->py;
    imgMeta[2] = res->max_iter;

    meta[0] = atof(res->cX);
    meta[1] = atof(res->cY);
    meta[2] = atof(res->Z);

	imgMeta_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * sizeof(int), NULL, &ret);
	meta_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * sizeof(double), NULL, &ret);
	data_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, res->px * res->py * sizeof(int), NULL, &ret);

	clEnqueueWriteBuffer(command_queue, imgMeta_buf, CL_TRUE, 0, 3 * sizeof(int), imgMeta, 0, NULL, NULL);

	clEnqueueWriteBuffer(command_queue, meta_buf, CL_TRUE, 0, 3 * sizeof(double), meta, 0, NULL, NULL);

	clEnqueueWriteBuffer(command_queue, data_buf, CL_TRUE, 0, res->px * res->py * sizeof(int), data, 0, NULL, NULL);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imgMeta_buf);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&meta_buf);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&data_buf);

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, data_buf, CL_TRUE, 0, res->px * res->py * sizeof(int), res->data, 0, NULL, NULL);

	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(data_buf);
	ret = clReleaseMemObject(meta_buf);
	ret = clReleaseMemObject(imgMeta_buf);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

}



