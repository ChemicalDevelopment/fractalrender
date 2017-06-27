/* engine_opencl/engine_opencl.h -- header file for OpenCL engine

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


#ifndef __ENGINE_OPENCL_H__
#define __ENGINE_OPENCL_H__

bool engine_opencl_isvalid;


void engine_opencl_error_handle(char *file, int line, char *src, int code);

bool engine_opencl_set_kernel__dev(int platid, int devid, bool doexit);

bool engine_opencl_set_kernel__plat(int platid, bool doexit, int clgdif);

void engine_opencl_set_kernel();



void engine_opencl_init(int __depth, int d0, int d1, int numincol, unsigned char * incol);

void engine_opencl_fulltest(fractal_img_t * ret);

void engine_opencl_end();


#endif
