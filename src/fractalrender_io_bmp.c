/* fractalrender_io_bmp.c -- store BMP files, doesn't use external libraries

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

#include "fractalrender_io_bmp.h"


fr_io_lib_export_t fr_io_export = {
    fr_io_bmp_dump
};

void fr_io_bmp_dump(fr_t * fr, FILE *fp) {
    log_debug("bmp io dump started");

    int filesize = 54 + 3 * fr->dim.width * fr->dim.height;

    unsigned char file_H[14] = {
        'B','M', 
        00, 00, 00, 00, 
        00, 00, 
        00, 00, 
        54, 00, 00, 00
    };

    unsigned char info_H[40] = {
        40, 00, 00, 00,
        00, 00, 00, 00,
        00, 00, 00, 00,
        01, 00,
        24, 00,
    };

    // for padding 
    unsigned char pad[4] = {
        00, 00, 00, 00
    };

    // set ints, 1 byte at a time
    file_H[ 2] = (unsigned char)(filesize);
    file_H[ 3] = (unsigned char)(filesize >>  8);
    file_H[ 4] = (unsigned char)(filesize >> 16);
    file_H[ 5] = (unsigned char)(filesize >> 24);

    // code to expand integers into byte format
    info_H[ 4] = (unsigned char)(fr->dim.width);
    info_H[ 5] = (unsigned char)(fr->dim.width >>  8);
    info_H[ 6] = (unsigned char)(fr->dim.width >> 16);
    info_H[ 7] = (unsigned char)(fr->dim.width >> 24);

    info_H[ 8] = (unsigned char)(fr->dim.height);
    info_H[ 9] = (unsigned char)(fr->dim.height >>  8);
    info_H[10] = (unsigned char)(fr->dim.height >> 16);
    info_H[11] = (unsigned char)(fr->dim.height >> 24);

    log_trace("bmp header writing started");

    fwrite(file_H, 1, sizeof(file_H), fp);
    fwrite(info_H, 1, sizeof(info_H), fp);


    log_trace("bmp header writing ended");

    //fwrite(fr->bitmap, 4, fr->dim.height * fr->dim.height, fp);


    log_trace("bmp pixel array writing started");

    int ri;

    int i, j;
    for (i = 0; i < fr->dim.height; i++) {
        for (j = 0; j < fr->dim.width; ++j) {
            ri = 4 * ((fr->dim.height - i - 1) * fr->dim.width + j);
            // instead of RGB, bmp files take BGR
            fwrite(&fr->bitmap[ri + 2], 1, 1, fp);
            fwrite(&fr->bitmap[ri + 1], 1, 1, fp);
            fwrite(&fr->bitmap[ri + 0], 1, 1, fp);
        }
        // pad the file to DWORD len
        fwrite(pad, 1, (4-(fr->dim.width*3)%4)%4, fp);
    }

    log_trace("bmp pixel array writing ended");

    log_debug("bmp io dump ended");

}


