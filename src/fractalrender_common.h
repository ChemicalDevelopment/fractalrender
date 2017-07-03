/* fractalrender_common.h -- defines types and common methods

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

#ifndef __FRACTALRENDER_COMMON_H__
#define __FRACTALRENDER_COMMON_H__

#include "fractalrender.h"

#define FR_COL_NONE        (0x0000)
#define FR_COL_RED         (0x0001)
#define FR_COL_RAND        (0x0101)


#define STR_EQ(a, b) (strcmp((a), (b)) == 0)
#define STR_EQIC(a, b) (strcicmp((a), (b)) == 0)

char * fr_get_ext(char *filename);

int strcicmp(char *a, char *b);

void fr_ctime(struct timeval *ret);

double fr_diffsec(struct timeval a, struct timeval b);


void fr_set_prop(fr_t * fr, char * name, char * val, double dval);

void fr_get_prop(char **_target, double *_target_d, fr_t * fr, char * name);

void fr_init(fr_t * ret, fr_dim_t dim);

void fr_clear(fr_t * ret);

void fr_set_col(fr_t * ret, fr_col_t col, char * color_scheme);

void fr_conv_prop(fr_t * fr);

FILE * fr_sfopen(char * fname, char * mode);

#endif


