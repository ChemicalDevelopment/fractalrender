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



void io_bmp_write_fractal(fractal_img_t * ret, FILE *fp) {
    int w = ret->px, h = ret->py;
    int i, j;
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int

    unsigned char file_H[14] = {
        'B','M', 
        0,0,0,0, 
        0,0, 
        0,0, 
        54,0,0,0
    };

    unsigned char info_H[40] = {
        40,0,0,0, 
        0,0,0,0, 
        0,0,0,0, 
        1,0, 
        24,0
    };


    // for 32DWORD lenght
    unsigned char pad[3] = {
        0,0,0
    };

    // set ints, 1 byte at a time
    file_H[2] = (unsigned char)(filesize);
    file_H[3] = (unsigned char)(filesize>>8);
    file_H[4] = (unsigned char)(filesize>>16);
    file_H[5] = (unsigned char)(filesize>>24);

    info_H[4] = (unsigned char)(w);
    info_H[5] = (unsigned char)(w>>8);
    info_H[6] = (unsigned char)(w>>16);
    info_H[7] = (unsigned char)(w>>24);

    info_H[8] = (unsigned char)(h);
    info_H[9] = (unsigned char)(h>>8);
    info_H[10] = (unsigned char)(h>>16);
    info_H[11] = (unsigned char)(h>>24);

    fwrite(file_H, 1, sizeof(file_H), fp);
    fwrite(info_H, 1, sizeof(info_H), fp);

    for(i = 0; i < h; i++)
    {
        for (j = 0; j < w; ++j) {
            // instead of RGB, bmp files take BGR
            fwrite(&ret->data[3*(i*w+j)+2], 1, 1, fp);
            fwrite(&ret->data[3*(i*w+j)+1], 1, 1, fp);
            fwrite(&ret->data[3*(i*w+j)+0], 1, 1, fp);
        }
        // pad the file to DWORD len
        fwrite(pad, 1, (4-(w*3)%4)%4, fp);
    }
}


