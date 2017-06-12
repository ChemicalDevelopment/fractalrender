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

color_src += $(color_c_src)




engine_c_src = engine_c/engine_c.h engine_c/engine_c.c
engine_mpf_src = engine_mpf/engine_mpf.h engine_mpf/engine_mpf.c
engine_mpc_src = engine_mpc/engine_mpc.h engine_mpc/engine_mpc.c
engine_opencl_src = engine_opencl/engine_opencl.h engine_opencl/engine_opencl.c

engine_src = 

engine_src += $(engine_c_src)


if HAVE_GMP
engine_src += $(engine_mpf_src)
endif

if HAVE_MPC
engine_src += $(engine_mpc_src)
endif

if HAVE_OPENCL
engine_src += $(engine_opencl_src)
endif


io_raw_src = io_raw/io_raw.h io_raw/io_raw.c
io_img_src = io_img/io_img.h io_img/io_img.c
io_png_src = io_png/io_png.h io_png/io_png.c

io_src =


io_src += $(io_raw_src)
io_src += $(io_img_src)

if HAVE_PNG
io_src += $(io_png_src)
endif

fractalrender_SOURCES = fractalrender.h alloc_lib.h tofile.h fractalrender.c alloc_lib.c tofile.c $(engine_src) $(io_src) $(color_src)
fractalrender_CFLAGS = @CL_CFLAGS@
fractalrender_LDADD = @CL_LIBS@
