/* fractalrender_engine_complex.h -- C complex engine

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


#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/opencl.h>
#endif

#ifdef HAVE_CL_CL_H
#include <CL/cl.h>
#endif

#ifdef HAVE_OPENCL_CL_H
#include <OpenCL/cl.h>
#endif


#ifndef __FRACTALRENDER_ENGINE_OPENCL_H__
#define __FRACTALRENDER_ENGINE_OPENCL_H__


typedef struct fr_ocl_t {
    int plat_id;
    int dev_id;

} fr_ocl_t;

void fr_engine_opencl_init(fr_t * fr);

void fr_engine_opencl_compute(fr_t * fr);

#endif
