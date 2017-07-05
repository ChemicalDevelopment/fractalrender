/* fractalrender_engine_mpf.h -- GMP mpf engine

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


#ifndef __FRACTALRENDER_ENGINE_MPF_H__
#define __FRACTALRENDER_ENGINE_MPF_H__

typedef struct e_mpf_fr_t {
    mpf_t cX, cY, Z;

    mpf_t s_c_i;

    mpf_t c_r, c_i;
    mpf_t z_r, z_i;
    mpf_t z_r2, z_i2;

    mpf_t tmp;

    mpf_t delta_ppx;

} e_mpf_fr_t;


void fr_engine_mpf_init(fr_t * fractal);

void fr_engine_mpf_compute(fr_t * fractal);

#endif
