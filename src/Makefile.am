###  src/Makefile.am -- Makefile for automake compiling main executables
#
#    Copyright 2016-2017 ChemicalDevelopment
#
#    This file is part of the FractalRender project
#
#    FractalRender source code, as well as any other resources in this project are
#  free software; you are free to redistribute it and/or modify them under
#  the terms of the GNU General Public License; either version 3 of the
#  license, or any later version.
#
#    These programs are hopefully useful and reliable, but it is understood
#  that these are provided WITHOUT ANY WARRANTY, or MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE. See the GPLv3 or email at
#  <info@chemicaldevelopment.us> for more info on this.
#
#    Here is a copy of the GPL v3, which this software is licensed under. You
#  can also find a copy at http://www.gnu.org/licenses/.
###

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = --pedantic -Wall -std=c99 -O2
AM_LDFLAGS =


# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = fractalrender



color_c_src = color_c/color_c.h color_c/color_c.c

color_src =

#color_src += $(color_c_src)




engine_c_src = engine_c/engine_c.h engine_c/engine_c.c
engine_cuda_src = engine_cuda/engine_cuda.h engine_cuda/engine_cuda.c engine_cuda/engine_cuda_kernel.cu engine_cuda/engine_cuda_impl.h engine_cuda/engine_cuda_helper.h
engine_complex_src = engine_complex/engine_complex.h engine_complex/engine_complex.c
engine_mpf_src = engine_mpf/engine_mpf.h engine_mpf/engine_mpf.c
engine_opencl_src = engine_opencl/engine_opencl.h engine_opencl/engine_opencl.c

engine_src =

engine_src += $(engine_c_src)
engine_src += $(engine_complex_src)

fractalrender_LDADD =

if HAVE_CUDA

NVCC_CFLAGS = -gencode arch=compute_53,code=sm_53 -gencode arch=compute_60,code=sm_60 -gencode arch=compute_62,code=sm_62 -gencode arch=compute_62,code=compute_62
CUDA_CFLAGS = @CUDA_CFLAGS@
CUDA_LDFLAGS = @CUDA_LDFLAGS@

frcuda.o: engine_cuda/engine_cuda_kernel.cu
	$(NVCC) $(CUDA_CFLAGS) $(NVCC_CFLAGS) -I. -I.. -DHAVE_CONFIG_H -m64 $(CUDA_LDFLAGS) -lib -o frcuda.o -c engine_cuda/engine_cuda_kernel.cu

frcuda_link.o: frcuda.o
	$(NVCC) $(CUDA_CFLAGS) $(NVCC_CFLAGS) -lib $(CUDA_LDFLAGS)-dc frcuda.o -lcudadevrt -lcudart -o frcuda_link.o


engine_src += $(engine_cuda_src)
fractalrender_LDADD += frcuda_link.o -lcudart -lcuda -lcudadevrt
endif

if HAVE_GMP
engine_src += $(engine_mpf_src)
endif


if HAVE_OPENCL
engine_src += $(engine_opencl_src)
endif

io_png_src = io_png/io_png.h io_png/io_png.c
io_bmp_src = io_bmp/io_bmp.h io_bmp/io_bmp.c

io_src =

io_src += $(io_bmp_src)

if HAVE_PNG
io_src += $(io_png_src)
endif

fractalrender_SOURCES = fractalrender.h fractalrender_defs.h alloc_lib.h tofile.h jobs.h fractalrender.c alloc_lib.c tofile.c jobs.c $(engine_src) $(io_src) $(color_src)
fractalrender_CFLAGS =
