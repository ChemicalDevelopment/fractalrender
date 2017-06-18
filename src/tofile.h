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


#define FR_FORMAT_COLOR                  (0x0101)

#define FR_FORMAT_PNG                    (0x0102)
#define FR_FORMAT_BMP                    (0x0103)


int get_format(char *filename);

void fractal_to_file(fractal_img_t *ret);

#endif
