/* c_engine/engine_c.h -- header file for C engine

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


#ifndef __ENGINE_CUDA_H__
#define __ENGINE_CUDA_H__

#include "engine_cuda/engine_cuda_impl.h"


bool engine_cuda_isvalid;

__cuda_fractal_t cfract;

void mand_cuda_init(__cuda_fractal_t * ret, fractal_img_t * reti);

void mand_cuda(__cuda_fractal_t * ret);

void mand_cuda_end();

void engine_cuda_fulltest(fractal_img_t * ret);

void engine_cuda_end();

#endif
