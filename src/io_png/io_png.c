/* io_raw.c -- .raw format input/output

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



void io_png_write_fractal(fractal_img_t * ret, FILE *fp) {
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int code;

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

    /*
    // Setup Exception handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error during png creation\n");
        code = 1;
        goto finalise;
    }
    */

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

    // SWITCH here:

//    tofile_c_png(ret, &png_ptr);


	png_bytep row;// = (png_bytep) malloc(3 * ret->px * sizeof(png_byte));

    int x, y;
    for (y = 0; y < ret->py; y++) {
        row = &ret->data[y * ret->px * 3];
        png_write_row(png_ptr, row);
    }

    /*

    FR_16BIT * data_16 = (FR_16BIT *)ret->data;
    FR_32BIT * data_32 = (FR_32BIT *)ret->data;
    FR_64BIT * data_64 = (FR_64BIT *)ret->data;

    FR_64BIT dataxy;



    int x, y;
    for (y = 0; y < ret->py; y++) {
        for (x = 0; x < ret->px; x++) {
            switch (ret->depth) {
                case 16: dataxy = data_16[y * ret->px + x]; break;
                case 32: dataxy = data_32[y * ret->px + x]; break;
                case 64: dataxy = data_64[y * ret->px + x]; break;
            }
            if (fr_ctype == FR_COLOR_RED_ONLY || fr_ctype == FR_COLOR_GREEN_ONLY || fr_ctype == FR_COLOR_BLUE_ONLY || fr_ctype == FR_COLOR_BW_LIN) {
                double c_xy = (double)dataxy / ret->max_iter;
                if (fr_ctype == FR_COLOR_RED_ONLY) {
                    row[3*x + 0] = (int)floor(255 * c_xy);
                    row[3*x + 1] = 0;
                    row[3*x + 2] = 0;
                   // row[3*x + 0] = dataxy % 256;
                   // row[3*x + 1] = (dataxy + 90) % 256;
                   // row[3*x + 1] = (dataxy + 90) % 256;
                   // row[3*x + 1] = (dataxy + 90) % 256;
                   // row[3*x + 2] = (dataxy + 180) % 256;
                } else if (fr_ctype == FR_COLOR_GREEN_ONLY) {
                    row[3*x + 0] = 0;
                    row[3*x + 1] = (int)floor(255 * c_xy);
                    row[3*x + 2] = 0;
                } else if (fr_ctype == FR_COLOR_BLUE_ONLY) {
                    row[3*x + 0] = 0;
                    row[3*x + 1] = 0;
                    row[3*x + 2] = (int)floor(255 * c_xy);
                } else if (fr_ctype == FR_COLOR_BW_LIN) {
                    row[3*x + 0] = (int)floor(255 * c_xy);
                    row[3*x + 1] = (int)floor(255 * c_xy);
                    row[3*x + 2] = (int)floor(255 * c_xy);
                }
            } else if (fr_ctype == FR_COLOR_BINARY) {
                row[3*x+0] = 255 * (dataxy == ret->max_iter);
                row[3*x+1] = 255 * (dataxy == ret->max_iter);
                row[3*x+2] = 255 * (dataxy == ret->max_iter);
            } else {
                printf("Unknown fractalrender color scheme!\n");
                exit(3);
            }
        }
        png_write_row(png_ptr, row);
    }

    */
    
    // End write
    png_write_end(png_ptr, NULL);

    finalise:
    //if (fp != NULL) fclose(fp);
    //if (row != NULL) free(row);
    if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

}


