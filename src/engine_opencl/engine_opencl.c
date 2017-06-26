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


bool engine_opencl_isvalid = false;


#define MAX_SOURCE_SIZE (0x100000)

#define CL_MAXPLATFORMS 10
#define CL_MAXDEVICES 10


cl_context context = NULL;
cl_command_queue command_queue = NULL;
cl_program program = NULL;
cl_kernel kernel = NULL;

cl_device_id device_id[CL_MAXDEVICES];
cl_uint res_num_devices;

cl_platform_id platform_id[CL_MAXPLATFORMS];
cl_uint res_num_platforms;

cl_int res;

cl_mem color_pattern_m, color_output_m;

bool cl_32, cl_64;


size_t *global_item_size;
size_t *local_item_size;

void engine_opencl_error_handle(char *file, int line, char *src, int code) {
    printf("OpenCL error code: %d\n", code);
    printf("%s:%d\n", file, line);
    printf("%s\n", src);
    exit(3);
}


#define CLGLBL_HNDL_S(ST, IS) ST; if (res) { IS; }

#define CLGLBL_HNDL(ST) CLGLBL_HNDL_S(ST, engine_opencl_error_handle(__FILE__, __LINE__, #ST, res))


void engine_opencl_init(int __depth, int d0, int d1, int numincol, unsigned char *incol) {

    if (engine_opencl_isvalid) {
        return;
    }
    engine_opencl_isvalid = true;

    cl_32 = cargs_get_flag("-CL32");
    cl_64 = cargs_get_flag("-CL64");

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
    

    CLGLBL_HNDL(res = clGetPlatformIDs(CL_MAXPLATFORMS, platform_id, &res_num_platforms));

    printf("OpenCL: found %d platforms\n", res_num_platforms);

    // select which device here

    char * cl_dev_type = cargs_get("-CLtype");
    long clgdif = CL_DEVICE_TYPE_DEFAULT;

    if (strcmp(cl_dev_type, "CPU") == 0) {
        clgdif = CL_DEVICE_TYPE_CPU;
    } else if (strcmp(cl_dev_type, "GPU") == 0) {
        clgdif = CL_DEVICE_TYPE_GPU;
    } else if (strcmp(cl_dev_type, "ALL") == 0) {
        clgdif = CL_DEVICE_TYPE_ALL;
    } else if (strcmp(cl_dev_type, "ACCELERATOR") == 0) {
        clgdif = CL_DEVICE_TYPE_ACCELERATOR;
    } else if (strcmp(cl_dev_type, "DEFAULT") == 0) {
        clgdif = CL_DEVICE_TYPE_DEFAULT;
    } else {
        printf("Unknown OpenCL device type: '%s' (try CPU, GPU, ALL, ACCELERATOR, DEFAULT)\n", cl_dev_type);
        FR_FAIL
    }

    long platform_id_idx = atoi(cargs_get("-CLplatform"));

    CLGLBL_HNDL(res = clGetDeviceIDs(platform_id[platform_id_idx], clgdif, CL_MAXDEVICES, device_id, &res_num_devices));

    printf("OpenCL: found %d devices on platform[0]\n", res_num_devices);

    CLGLBL_HNDL(context = clCreateContext(0, res_num_devices, device_id, NULL, NULL, &res));
    
    CLGLBL_HNDL(command_queue = clCreateCommandQueue(context, device_id[atoi(cargs_get("-CLdevice"))], 0, &res));

    CLGLBL_HNDL(program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &res));

    CLGLBL_HNDL(res = clBuildProgram(program, res_num_devices, device_id, NULL, NULL, NULL));


    // switch based on kernel
    char *kernel_name;

    if (cl_32 && !cl_64) {
        kernel_name = "mand_32";
    } else {
        kernel_name = "mand";
    }

    CLGLBL_HNDL_S(kernel = clCreateKernel(program, kernel_name, &res),
        if (res == -46) {
            if (cl_64 && !cl_32) {
                printf("You specified -CL64, but kernel `mand` was not found. This normally means that your device does not support 64bit. Try omitting `-CL64`, or adding `-CL32` and rerunning\n");
                FR_FAIL
            } else {
                kernel_name = "mand_32";
                cl_64 = false;
                cl_32 = true;
                CLGLBL_HNDL_S(kernel = clCreateKernel(program, kernel_name, &res),
                    printf("Tried kernels `mand` and `mand_32`, and neither worked.\n");
                    FR_FAIL
                );
                printf("Found 32 bit kernel, using it now. To force 64 bit, use `-CL64`. (successful finding kernel)\n");
            }
        } else {
            CLGLBL_HNDL(printf("Error finding kernel\n"); );
            FR_FAIL
        }
    );

	global_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	local_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	global_item_size[0] = d0;
	global_item_size[1] = d1;
	local_item_size[0] = cargs_get_int_idx("-CLsize", 0);
	local_item_size[1] = cargs_get_int_idx("-CLsize", 1);

    CLGLBL_HNDL(color_pattern_m = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * numincol, NULL, &res));

	CLGLBL_HNDL(clEnqueueWriteBuffer(command_queue, color_pattern_m, CL_TRUE, 0, 3 * numincol, incol, 0, NULL, NULL));

    CLGLBL_HNDL(color_output_m = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * global_item_size[0] * global_item_size[1], NULL, &res));
	
}


void engine_opencl_end() {
    if (engine_opencl_isvalid) {
        CLGLBL_HNDL(res = clFlush(command_queue));
        CLGLBL_HNDL(res = clFinish(command_queue));
        CLGLBL_HNDL(res = clReleaseKernel(kernel));
        CLGLBL_HNDL(res = clReleaseProgram(program));
        CLGLBL_HNDL(res = clReleaseMemObject(color_pattern_m));
        CLGLBL_HNDL(res = clReleaseMemObject(color_output_m));
        CLGLBL_HNDL(res = clReleaseCommandQueue(command_queue));
        CLGLBL_HNDL(res = clReleaseContext(context));
    }
}


void engine_opencl_fulltest(fractal_img_t * ret) {

    if (!engine_opencl_isvalid) {
        engine_opencl_init(ret->depth, ret->px, ret->py, ret->color.numcol, ret->color.data);
    }

    long long *data = (long long *)ret->data;

    double cX, cY, Z;

    float cXf, cYf, Zf;

    cX = atof(ret->cX);
    cY = atof(ret->cY);
    Z = atof(ret->Z);
    cXf = (float)cX;
    cYf = (float)cY;
    Zf = (float)Z;

    float cdf, cmf;

    cdf = (float)ret->color.disp;
    cmf = (float)ret->color.mult;

	//clEnqueueWriteBuffer(command_queue, data_buf, CL_TRUE, 0, ret->px * ret->py * sizeof(FR_16BIT), data, 0, NULL, NULL);

    CLGLBL_HNDL(res = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&color_pattern_m));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 1, sizeof(int), &ret->color.numcol));
    if (cl_32) {
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 2, sizeof(float), &cmf));
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 3, sizeof(float), &cdf));
    } else {
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 2, sizeof(double), &ret->color.mult));
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 3, sizeof(double), &ret->color.disp));
    }
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 4, sizeof(int), &ret->color.is_simple));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&color_output_m));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 6, sizeof(int), &ret->px));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 7, sizeof(int), &ret->py));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 8, sizeof(int), &ret->max_iter));

    if (cl_32) {
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 9, sizeof(float), &cXf));
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 10, sizeof(float), &cYf));
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 11, sizeof(float), &Zf));
        float ctimef = ret->ctime; 
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 12, sizeof(float), &ctimef));
    } else {
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 9, sizeof(double), &cX));
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 10, sizeof(double), &cY));
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 11, sizeof(double), &Z));
        CLGLBL_HNDL(res = clSetKernelArg(kernel, 12, sizeof(double), &ret->ctime));
    }

    CLGLBL_HNDL_S(res = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL),
        if (res == -54) {
            if (!cargs_get_flag("-CLsize")) {
                local_item_size[0] = 1;
                local_item_size[1] = 1;
                CLGLBL_HNDL_S(res = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL),
                    printf("Tried size 1x1, and that still failed. Report this!\n");
                    FR_FAIL
                );
            } else {
                printf("ERROR: Local item size (-CLsize) doesn't work! Try removing the option\n");
                FR_FAIL
            }
        } else {
            CLGLBL_HNDL();
        }
    );

    CLGLBL_HNDL(res = clEnqueueReadBuffer(command_queue, color_output_m, CL_TRUE, 0, ret->px * ret->py * 3, ret->data, 0, NULL, NULL));

}



