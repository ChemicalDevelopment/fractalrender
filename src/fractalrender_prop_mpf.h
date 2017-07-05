/* fractalrender_prop_mpf.c -- the MPF implementation of the properties handler

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

#include <gmp.h>

#ifndef __FRACTALRENDER_PROP_MPF_H__
#define __FRACTALRENDER_PROP_MPF_H__

void fr_prop_init(fr_t * fr);

void fr_zoomin(fr_t * fr, double scale);

void fr_movex(fr_t * fr, double scale);

void fr_movey(fr_t * fr, double scale);

void fr_set_prop(fr_t * fr, char * name, char * val, double dval);

void fr_get_prop(char **_target, double *_target_d, fr_t * fr, char * name);

void fr_set_prec(fr_t * fr, int prec);

#endif
