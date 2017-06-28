/* c_engine/engine_c.c -- C engine for computing fractals

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


bool engine_cuda_isvalid = false;

__cuda_fractal_t cfract;


void engine_cuda_fulltest(fractal_img_t * ret) {
  if (!engine_cuda_isvalid) {
      cfract.cuda_size_x = cargs_get_int_idx("-CUDAsize", 0);
      cfract.cuda_size_y = cargs_get_int_idx("-CUDAsize", 1);
      mand_cuda_init(&cfract, ret);
      gettimeofday(&scl, NULL);
      engine_cuda_isvalid = true;
  }
  //mand_cuda(&ret->data, ret->px, ret->py, ret->max_iter, atof(ret->cX), atof(ret->cY), atof(ret->Z));
  cfract._gpu_fractal.Z = atof(ret->Z);
  cfract._gpu_fractal.simplecolor = ret->color.is_simple;
  mand_cuda(&cfract);

}

void engine_cuda_end() {
    mand_cuda_end(&cfract);
}
