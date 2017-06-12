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



void io_raw_read_fractal(fractal_img_t * ret, FILE *fp) {
    // todo malloc fscanf params
    int x, y;

    FR_64BIT dataxy;
    int fsfr;

    ret->engine = (char *)malloc(FR_MAX_PARAMSTRLEN);
    ret->cX = (char *)malloc(FR_MAX_PARAMSTRLEN);
    ret->cY = (char *)malloc(FR_MAX_PARAMSTRLEN);
    ret->Z = (char *)malloc(FR_MAX_PARAMSTRLEN);

    fsfr = fscanf(fp, "fractalrender_raw_file\n");
    fsfr = fscanf(fp, "px %ld\n", &ret->px);
    fsfr = fscanf(fp, "py %ld\n", &ret->py);

    fsfr = fscanf(fp, "engine %s\n", ret->engine);

    fsfr = fscanf(fp, "prec %ld\n", &ret->prec);
    fsfr = fscanf(fp, "depth %ld\n", &ret->depth);
    fsfr = fscanf(fp, "max_iter %ld\n", &ret->max_iter);

    fsfr = fscanf(fp, "cX %s\n", ret->cX);
    fsfr = fscanf(fp, "cY %s\n", ret->cY);

    fsfr = fscanf(fp, "Z %s\n", ret->Z);

    fsfr = fscanf(fp, "::::");

    ret->data = (void *)malloc(ret->px*ret->py * ret->depth/8);

    if (ret->depth == 16) {
        fsfr = fread(ret->data, ret->depth/8, ret->px*ret->py, fp);
    } else if (ret->depth == 32) {
        fsfr = fread(ret->data, ret->depth/8, ret->px*ret->py, fp);
    } else if (ret->depth == 64) {
        fsfr = fread(ret->data, ret->depth/8, ret->px*ret->py, fp);
    } else {
        printf("Unknown depth: %ld\n", ret->depth);
    }
}


void io_raw_write_fractal(fractal_img_t * ret, FILE *fp) {
    int x, y;

    FR_16BIT * data_16 = (FR_16BIT *)ret->data;
    FR_32BIT * data_32 = (FR_32BIT *)ret->data;
    FR_64BIT * data_64 = (FR_64BIT *)ret->data;


    FR_64BIT dataxy;

    fprintf(fp, "fractalrender_raw_file\n");
    fprintf(fp, "px %ld\n", ret->px);
    fprintf(fp, "py %ld\n", ret->py);

    fprintf(fp, "engine %s\n", ret->engine);

    fprintf(fp, "prec %ld\n", ret->prec);
    fprintf(fp, "depth %ld\n", ret->depth);
    fprintf(fp, "max_iter %ld\n", ret->max_iter);

    fprintf(fp, "cX %s\n", ret->cX);
    fprintf(fp, "cY %s\n", ret->cY);
    fprintf(fp, "Z %s\n", ret->Z);

    fprintf(fp, "::::");

    if (ret->depth == 16) {
        fwrite(data_16, ret->depth/8, ret->px*ret->py, fp);
    } else if (ret->depth == 32) {
        fwrite(data_32, ret->depth/8, ret->px*ret->py, fp);
    } else if (ret->depth == 64) {
        fwrite(data_64, ret->depth/8, ret->px*ret->py, fp);
    } else {
        printf("Unknown depth: %ld\n", ret->depth);
    }
}



