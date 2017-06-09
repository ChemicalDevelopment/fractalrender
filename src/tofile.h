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


#ifndef __TOFILE_H__
#define __TOFILE_H__


#include "fractalrender.h"

#define FR_COLOR_RED_ONLY                (0x0001)
#define FR_COLOR_BLUE_ONLY               (0x0002)
#define FR_COLOR_GREEN_ONLY              (0x0003)
#define FR_COLOR_BW_LIN                  (0x0004)
#define FR_COLOR_BINARY                  (0x0005)


int fractal_to_raw(fractal_img_t *ret);

int fractal_to_png(fractal_img_t *ret);

int fractal_to_file(fractal_img_t *ret);


#endif
