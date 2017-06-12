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

#define FR_ENDSWITH(a, b) (strlen(a) >= strlen(b) && strcmp(a + strlen(a) - strlen(b), b) == 0)

int get_format(char *filename) {
    if (FR_ENDSWITH(filename, ".png")) {
        return FR_FORMAT_PNG;
    } else if (FR_ENDSWITH(filename, ".raw")) {
        return FR_FORMAT_RAW;
    } else {
        printf("Don't know what format for file: %s\n", filename);
        exit(3);
    }
}

void fractal_to_file(fractal_img_t *ret, img_t *reti) {
    int fmt = get_format(ret->out);
    if (fmt == FR_FORMAT_RAW) {
        FILE *fp = sfopen(ret->out, "wb");

        io_raw_write_fractal(ret, fp);
        fclose(fp);
    } else if (fmt == FR_FORMAT_PNG) {
        #ifdef HAVE_PNG
        FILE *fp = sfopen(ret->out, "wb");
        img_t reti;
        io_init_fractal_to_img(ret, &reti);

        color_c_fractal_to_img(ret, &reti);
        io_png_write_fractal(&reti, fp);
        fclose(fp);
        #else
        printf("ERROR: wasn't compiled with support for .png\n");
        exit(3);
        #endif
    } else {
        printf("Unknown output format\n");
        exit(3);
    }
}


