/* fractalrender_color.h -- common color utilities

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

#ifndef __FRACTALRENDER_COLOR_H__
#define __FRACTALRENDER_COLOR_H__


#define FRVC(C) ((C) >= 0 && (C) <= 255)
#define FR_VALID_RGBA(R, G, B, A) (FRVC(R) && FRVC(G) && FRVC(B) && FRVC(A))


void fr_col_gen_red(int idx, fr_col_t * ret);

void fr_col_gen_green(int idx, fr_col_t * fr);

void fr_col_gen_blue(int idx, fr_col_t * fr);

void fr_col_gen_mocha(int idx, fr_col_t * fr);

void fr_col_gen_random(int idx, fr_col_t * fr);

void fr_col_fillinidx(int ci, double zn2, int ri, fr_t * fr);

#endif
