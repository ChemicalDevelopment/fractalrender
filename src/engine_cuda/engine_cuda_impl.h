/* cuda_engine/cuda_engine_impl.h -- header file for CUDA engine

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


#ifndef __ENGINE_CUDA_IMPL_H__
#define __ENGINE_CUDA_IMPL_H__


typedef struct __cuda_gpu_fractal_t {
    double cX, cY, Z;
    int px, py;

    int simplecolor;

    double colm, cold;

    int numcol;

    int max_iter;
} __cuda_gpu_fractal_t;


typedef struct __cuda_fractal_t {

    __cuda_gpu_fractal_t _gpu_fractal;

    int cuda_size_x, cuda_size_y;

    unsigned char * incol;
    unsigned char * outcol;

    unsigned char * _gpu_incol;
    unsigned char * _gpu_outcol;

} __cuda_fractal_t;


#endif
