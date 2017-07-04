/* fractalrender_engine_c.c -- C engine

  Copyright 2016-2017 ChemicalDevelopment

  This file is part of the fractalrender project.

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

#include "fractalrender_engine_opencl.h"

#define MAX_SOURCE_SIZE (0x100000)

#define CL_MAXPLATFORMS 10
#define CL_MAXDEVICES 10

#define CLGLBL_HNDL_S(ST, IS, ES) ST; if (res) { IS; } else { ES; }

#define CLGLBL_HNDL(ST) CLGLBL_HNDL_S(ST, fr_engine_opencl_error_handle(__FILE__, __LINE__, #ST, res), )


fr_engine_lib_export_t fr_engine_opencl_export = {
    fr_engine_opencl_init,
    fr_engine_opencl_compute
};


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
bool using_cl_32;

char *kernelname;

char *source_str;
size_t source_size;

size_t *global_item_size;
size_t *local_item_size;


fr_ocl_t fr_ocl;


void fr_engine_opencl_error_handle(char *file, int line, char *src, int code) {
    log_warn("OpenCL error code: %d\n", code);
    log_warn("%s:%d\n", file, line);
    log_fatal("%s\n", src);
}


bool engine_opencl_set_kernel__dev(int platid, int devid, bool doexit) {
    log_debug("Trying platform[%d], device[%d]\n", platid, devid);

    CLGLBL_HNDL(command_queue = clCreateCommandQueue(context, device_id[devid], 0, &res));

    CLGLBL_HNDL(program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &res));

    CLGLBL_HNDL(res = clBuildProgram(program, res_num_devices, device_id, NULL, NULL, NULL));

    log_debug("looking for kernel `%s`...", kernelname);

    CLGLBL_HNDL_S(kernel = clCreateKernel(program, kernelname, &res),

        if (cl_32 && !cl_64) {
            log_error("Couldn't find kernel `mand_32`, try omitting all `-CLXX` arguments\n");
            if (doexit) {
                FR_FAIL
            }
            return false;
        } else if (cl_64 && !cl_32) {
            log_error("You requested 64 bit (using `-CL64`), but could not find a 64 bit kernel\n");
            if (doexit) {
                FR_FAIL
            }
            return false;
        } else {
            kernelname = "mand_32";
            using_cl_32 = true;

            log_debug("looking for kernel `%s`...", kernelname);

            CLGLBL_HNDL_S(kernel = clCreateKernel(program, kernelname, &res),

                log_fatal("Couldn't find kernel `mand_32`, try omitting all `-CLXX` arguments\n");
                if (doexit) {
                    FR_FAIL
                }
                return false;
                ,
                log_debug("found");
                return true;
            )
        }
        ,
        log_debug("found");
        return true;
    );
}


bool engine_opencl_set_kernel__plat(int platid, bool doexit, int clgdif) {
    log_debug("Trying platform[%d]\n", platid);
    CLGLBL_HNDL_S(res = clGetDeviceIDs(platform_id[platid], clgdif, CL_MAXDEVICES, device_id, &res_num_devices),
        if (res == -1) {
            log_debug("Device not found for platform[%d]\n", platid);
            if (doexit) {
                FR_FAIL
            }
            return false;
        } else {
            CLGLBL_HNDL()
        }
    ,
        //
    );
    log_debug("OpenCL: found %d devices on platform[%d]\n", res_num_devices, platid);
    int devid;
    CLGLBL_HNDL(context = clCreateContext(0, res_num_devices, device_id, NULL, NULL, &res));
    if (fr_ocl.dev_id >= 0) {
        devid = fr_ocl.dev_id;
        if (!engine_opencl_set_kernel__dev(platid, devid, true)) {
            if (doexit) {
                FR_FAIL
            }
            return false;
        } else {
            return true;
        }
    } else {
        for (devid = 0; devid < res_num_devices; ++devid) {
            if (engine_opencl_set_kernel__dev(platid, devid, false)) {
                return true;
            }
        }
        if (doexit) {
            FR_FAIL
        }
        return false;
    }
}


void engine_opencl_print_help() {
    printf("  -k             kernel");
    printf("  -b             bit kernel (32 or 64)");
    printf("  -h             print help");
    printf("\n");
}

void engine_opencl_set_kernel(fr_t * fr) {
    char c;

    char * kernel_file = NULL;

    optind = 1;    

    if (fr->argc > 1) {
        while ((c = getopt (fr->argc, fr->argv, "k:b:h")) != -1) {
            switch (c) {
                case 'k':
                    kernel_file = optarg;
                    break;
                case 'b':
                    if (STR_EQ(optarg, "64")) {
                        cl_64 = true;
                    } else if (STR_EQ(optarg, "32")) {
                        cl_32 = true;
                    } else {
                        printf("incorrect kernel bit size: %s\n", optarg);
                    }
                    break;
                case 'h':
                    engine_opencl_print_help();
                    FR_FINISH
                    break;
                case '?':
                    engine_opencl_print_help();
                    break;
                default:
                    log_fatal("unknown getopt");
                    break;
            }
        }
    }
    if (!cl_32 && !cl_64) {
        cl_32 = true;
        cl_64 = true;
    }

    int platid;

    if (cl_32 && !cl_64) {
        kernelname = "mand_32";
        using_cl_32 = true;
    } else {
        kernelname = "mand";
        using_cl_32 = false;
    }


    log_trace("kernel_file: %s", kernel_file);
    log_trace("32b: %d, 64b: %d", cl_32, cl_64);



    char * cl_dev_type = "GPU";
    long clgdif = CL_DEVICE_TYPE_DEFAULT;


    if (kernel_file != NULL) {
        FILE *fp = fr_sfopen(kernel_file, "r");
        source_str = (char*)malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);
    } else {
        //source_str = ENGINE_OPENCL_KERNEL_SOURCE_DEFAULT;
        source_size = strlen(source_str);
    }

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

    printf("Looking for %s devices\n", cl_dev_type);

    CLGLBL_HNDL(res = clGetPlatformIDs(CL_MAXPLATFORMS, platform_id, &res_num_platforms));

    printf("OpenCL: found %d platforms\n", res_num_platforms);


    if (fr_ocl.plat_id >= 0) {
        platid = fr_ocl.plat_id;
        if (engine_opencl_set_kernel__plat(platid, true, clgdif)) {
            return;
        } else {
            FR_FAIL
        }
    } else {
        for (platid = 0; platid < res_num_platforms; ++platid) {
            if (engine_opencl_set_kernel__plat(platid, false, clgdif)) {
                return;
            }
        }
        FR_FAIL
    }
    FR_FAIL
    return;
}





void fr_engine_opencl_init(fr_t * fr) {
    log_debug("opencl engine initialized");
    engine_opencl_set_kernel(fr);


    global_item_size = (size_t *)malloc(sizeof(size_t) * 2);
    local_item_size = (size_t *)malloc(sizeof(size_t) * 2);
    global_item_size[0] = fr->dim.width;
    global_item_size[1] = fr->dim.height;
    local_item_size[0] = 16;
    local_item_size[1] = 16;


    CLGLBL_HNDL(color_pattern_m = clCreateBuffer(context, CL_MEM_READ_WRITE, fr->dim.byte_depth * fr->col.col_len, NULL, &res));

    CLGLBL_HNDL(clEnqueueWriteBuffer(command_queue, color_pattern_m, CL_TRUE, 0, fr->dim.byte_depth * fr->col.col_len, fr->col.in_col, 0, NULL, NULL));

    CLGLBL_HNDL(color_output_m = clCreateBuffer(context, CL_MEM_READ_WRITE, fr->dim.mem_width * fr->dim.height, NULL, &res));


}

void fr_engine_opencl_compute(fr_t * fr) {
    log_debug("opencl engine computing started");

    double cX = fr->prop.center_x, cY = fr->prop.center_y, Z = fr->prop.zoom;

    float cXf = (float)cX, cYf = (float)cY, Zf = (float)Z;


    float cdf, cmf;

    cdf = (float)fr->col.offset;
    cmf = (float)fr->col.scale;

    //__kernel void mand(double center_x, double center_y, double zoom, int height, int width, int mem_width, double er2, int max_iter, int is_simple, double col_scale, double col_offset, int byte_depth, int col_len, __global uchar * in_col, __global uchar * bitmap)

    log_trace("opencl engine: center_x: %lf, center_y: %lf, zoom: %lf", fr->prop.center_x, fr->prop.center_y, fr->prop.zoom);


    CLGLBL_HNDL(res = clSetKernelArg(kernel, 0, sizeof(double), &fr->prop.center_x));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 1, sizeof(double), &fr->prop.center_y));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 2, sizeof(double), &fr->prop.zoom));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 3, sizeof(int), &fr->dim.height));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 4, sizeof(int), &fr->dim.width));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 5, sizeof(int), &fr->dim.mem_width));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 6, sizeof(double), &fr->prop.er2));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 7, sizeof(int), &fr->prop.max_iter));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 8, sizeof(int), &fr->col.is_simple));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 9, sizeof(double), &fr->col.scale));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 10, sizeof(double), &fr->col.offset));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 11, sizeof(int), &fr->dim.byte_depth));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 12, sizeof(int), &fr->col.col_len));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 13, sizeof(cl_mem), (void *)&color_pattern_m));
    CLGLBL_HNDL(res = clSetKernelArg(kernel, 14, sizeof(cl_mem), (void *)&color_output_m));


    printf("trying local work size [%zu, %zu]...", local_item_size[0], local_item_size[1]);
    CLGLBL_HNDL_S(res = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL),
        if (res == -54) {
            printf("failed\n");

            if (!false) {
                local_item_size[0] = 1;
                local_item_size[1] = 1;

                printf("trying local work size [%zu, %zu]...", local_item_size[0], local_item_size[1]);

                CLGLBL_HNDL_S(res = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL),
                    printf("failed\n");
                    printf("size [1, 1] failed. Report this incident!\n");
                    FR_FAIL
                ,
                    printf("worked\n");
                );
            } else {
                printf("ERROR: Local item size (-CLsize) doesn't work! Try removing the option\n");
                FR_FAIL
            }
        } else {
            CLGLBL_HNDL();
        }
    ,
        printf("worked\n");
    );

    log_trace("now reading from OpenCL buffer");

    //fr->bitmap = (unsigned char *)malloc(fr->dim.mem_width * fr->dim.height);
    CLGLBL_HNDL(res = clEnqueueReadBuffer(command_queue, color_output_m, CL_TRUE, 0, fr->dim.mem_width * fr->dim.height, fr->bitmap, 0, NULL, NULL));


    log_debug("opencl engine computing ended");
}

