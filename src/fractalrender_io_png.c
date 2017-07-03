/* fractalrender_io_png.h -- store PNG files, and is loaded dynamically

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

#include "fractalrender_io_png.h"

#include <png.h>

fr_io_lib_export_t fr_io_export = {
    fr_io_png_dump
};


void fr_io_png_dump(fr_t * fr, FILE *fp) {
    log_debug("png io dump started");

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // Initialize write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        log_error("Couldn't create PNG write struct");
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        log_error("Couldn't create PNG info struct");
    }

    // Setup Exception handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        log_error("Error during PNG creation");
    }

    png_init_io(png_ptr, fp);

    log_trace("png header writing started");

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, fr->dim.width, fr->dim.height,
        8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    log_trace("png header ended started");


    char * title = "fractalrender";

    // Set title
    if (title != NULL) {
        png_text title_text;
        title_text.compression = PNG_TEXT_COMPRESSION_NONE;
        title_text.key = "Title";
        title_text.text = title;
        png_set_text(png_ptr, info_ptr, &title_text, 1);
    }

    log_trace("png info writing started");

    png_write_info(png_ptr, info_ptr);

    log_trace("png info writing ended");

    log_trace("png pixel writing started");

    int y;
    for (y = 0; y < fr->dim.height; y++) {
        png_write_row(png_ptr, fr->bitmap + 4 * fr->dim.width * y);
    }
    
    log_trace("png pixel writing ended");


    // End write
    png_write_end(png_ptr, NULL);

    if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    log_debug("png io dump ended");

}

