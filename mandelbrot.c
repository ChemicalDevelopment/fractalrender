#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#include <math.h>

#define MAX_SOURCE_SIZE (0x100000)

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
cl_mem data_png_buf = NULL;

size_t *global_item_size;
size_t *local_item_size;

//Buffers
int * imgMeta;
double * meta;
int * data;
float * data_png;
//Constants
int start_frame;
int end_frame;
int max_frame;
int width;
int height;
int maxIter;
double centerX;
double centerY;
double zoomps;


//Things that will change
double zoom;
double seconds;

//Call this with ./mandelbrot.o startframe endframe maxframe seconds width height maxIter centerX centerY zoom zoomps
void init(char *argv[]) {
	start_frame = strtol(argv[1], NULL, 0);
	end_frame = strtol(argv[2], NULL, 0);
	max_frame = strtol(argv[3], NULL, 0);
	seconds = strtol(argv[4], NULL, 0);
	width = strtol(argv[5], NULL, 0);
	height = strtol(argv[6], NULL, 0);
	maxIter = strtol(argv[7], NULL, 0);
	centerX = strtod(argv[8], NULL);
	centerY = strtod(argv[9], NULL);
	zoom = strtod(argv[10], NULL);
	zoomps = strtod(argv[11], NULL);
}
void setRGB(png_byte *ptr, double val)
{
	int v = (int)(val * 767);
	if (v < 0) v = 0;
	if (v > 767) v = 767;
	int offset = v % 256;

	if (v<256) {
		ptr[0] = 0; ptr[1] = 0; ptr[2] = offset;
	}
	else if (v<512) {
		ptr[0] = 0; ptr[1] = offset; ptr[2] = 255-offset;
	}
	else {
		ptr[0] = offset; ptr[1] = 255-offset; ptr[2] = 0;
	}
}

int writeImage(char* filename, int width, int height, float *buffer, char* title)
{
	int code = 0;
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep row = NULL;
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	float v;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width ; x++) {
			v = buffer[y*width + x];
			setRGB(&(row[x*3]), v);
			setRGB(&(row[x*3+1]), v*v);
			setRGB(&(row[x*3+2]), v*v*v);
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}

void returnIterArray(int frame) {
	
	/* Execute OpenCL Kernel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL);
	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, data_png_buf, CL_TRUE, 0, width * height * sizeof(float), data_png, 0, NULL, NULL);

	char fn[120];
	snprintf(fn, sizeof fn, "./tmp/file%d.png", frame);
	writeImage(fn, width, height, data_png, "Mandelbrot");

}

int main(int argc, char *argv[])
{
	init(argv);
	FILE *fp;
	char fileName[] = "./kernel.cl";
	char *source_str;
	size_t source_size;
	
	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	
	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	
	//Get device ID's'
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
	
	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	
	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &ret);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	
	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "mand", &ret);\

	//global_item_size = width * height;
	//local_item_size = 1;
	global_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	local_item_size = (size_t *)malloc(sizeof(size_t) * 2);
	global_item_size[0] = width;
	global_item_size[1] = height;
	local_item_size[0] = 16;
	local_item_size[1] = 16;

	int i;
	double time = 0;
	double base_zoom = zoom;

	printf("Running OpenCL\n");

	imgMeta = (int *)malloc(3 * sizeof(int));
	meta = (double *)malloc(3 * sizeof(double));
	data = (int *)malloc(width * height * sizeof(int));
	data_png = (float *)malloc(height * width * sizeof(float));

	//width, height, maxiter
	imgMeta[0] = width;
	imgMeta[1] = height;
	imgMeta[2] = maxIter;
	//center x, y, zoom
	meta[0] = centerX;
	meta[1] = centerY;
	meta[2] = zoom;

	/* Create Buffers */
	imgMeta_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * sizeof(int), NULL, &ret);
	data_png_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, width * height * sizeof(float), NULL, &ret);
	meta_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, 3 * sizeof(double), NULL, &ret);
	
	/* Write all buffers */
	clEnqueueWriteBuffer(command_queue, imgMeta_buf, CL_TRUE, 0, 3 * sizeof(int), imgMeta, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, data_png_buf, CL_TRUE, 0, width * height * sizeof(float), data_png, 0, NULL, NULL);

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imgMeta_buf);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&data_png_buf);


	for (i = start_frame; i < end_frame; ++i) {
		time = (i * seconds) / (max_frame - 1);
		meta[2] = base_zoom * pow(zoomps, time);
		clEnqueueWriteBuffer(command_queue, meta_buf, CL_TRUE, 0, 3 * sizeof(double), meta, 0, NULL, NULL);
		ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&meta_buf);
		returnIterArray(i);
		printf("%%%d Done\r", (100 * (i - start_frame)) / (end_frame - start_frame - 1));
		fflush(stdout);
	}

	printf("\nDone.\n");

	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(data_png_buf);
	ret = clReleaseMemObject(meta_buf);
	ret = clReleaseMemObject(imgMeta_buf);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	
	free(source_str);
	
	return 0;
}
