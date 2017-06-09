/* tofile.c -- outputs fractal images to files

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



int fractal_to_file(fractal_img_t *ret, char * filename) {
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
    png_set_IHDR(png_ptr, info_ptr, ret->px, ret->py,
        8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    char * title = "fractalrender";

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
    row = (png_bytep) malloc(3 * ret->px * sizeof(png_byte));

    FR_1BIT * data_1 = (FR_1BIT *)ret->data;
    FR_8BIT * data_8 = (FR_8BIT *)ret->data;
    FR_16BIT * data_16 = (FR_16BIT *)ret->data;
    FR_32BIT * data_32 = (FR_32BIT *)ret->data;
    FR_64BIT * data_64 = (FR_64BIT *)ret->data;

    // Write image data
    int x, y;
    for (y = 0; y < ret->py; y++) {
        for (x = 0; x < ret->px; x++) {
            switch (ret->depth) {
                // todo optimize case 1
                case 1: row[3*x + 0] = FR_8BIT_MAX * data_1[y * ret->px + x]; break;
                case 8: row[3*x + 0] = data_8[y * ret->px + x]; break;
                case 16: row[3*x + 0] = data_16[y * ret->px + x] / FR_8BIT_MAX; break;
                case 32: row[3*x + 0] = data_32[y * ret->px + x] / (FR_8BIT_MAX*FR_16BIT_MAX); break;
                case 64: row[3*x + 0] = data_64[y * ret->px + x] / (FR_8BIT_MAX*FR_16BIT_MAX*FR_32BIT_MAX); break;
            }
            //row[3*x + 0] = ((char *)ret->data)[y*ret->px + x];
            row[3*x + 1] = 0;
            row[3*x + 2] = 0;
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

