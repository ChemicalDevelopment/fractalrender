###  src/Makefile.am -- Makefile for automake compiling main executables
#
#    Copyright 2016-2017 ChemicalDevelopment
#
#    This file is part of the fractalrender project
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

lib_LTLIBRARIES = libfr_common.la libfr_io_bmp.la libfr_engine_c.la libfr_engine_complex.la libfr_prop_c.la

if HAVE_GMP
lib_LTLIBRARIES += libfr_engine_mpf.la libfr_prop_mpf.la
endif

if HAVE_PNG
lib_LTLIBRARIES += libfr_io_png.la
endif

if HAVE_SDL
lib_LTLIBRARIES += libfr_interactive_sdl.la
endif

if HAVE_OPENCL
lib_LTLIBRARIES += libfr_engine_opencl.la
endif


bin_PROGRAMS = fractalrender


###

# C engine
libfr_engine_c_la_SOURCES = fractalrender_engine_c.h fractalrender_engine_c.c
libfr_engine_c_la_LDFLAGS =
libfr_engine_c_la_LIBADD = libfr_common.la


# C prop lib
libfr_prop_c_la_SOURCES = fractalrender_prop_c.h fractalrender_prop_c.c
libfr_prop_c_la_LDFLAGS =
libfr_prop_c_la_LIBADD = libfr_common.la

# C complex engine
libfr_engine_complex_la_SOURCES = fractalrender_engine_complex.h fractalrender_engine_complex.c
libfr_engine_complex_la_LDFLAGS =
libfr_engine_complex_la_LIBADD = libfr_common.la

# GMP mpf complex engine
libfr_engine_mpf_la_SOURCES = fractalrender_engine_mpf.h fractalrender_engine_mpf.c
libfr_engine_mpf_la_LDFLAGS = -lgmp
libfr_engine_mpf_la_LIBADD = libfr_common.la

# GMP mpf properties
libfr_prop_mpf_la_SOURCES = fractalrender_prop_mpf.h fractalrender_prop_mpf.c
libfr_prop_mpf_la_LDFLAGS = -lgmp
libfr_prop_mpf_la_LIBADD = libfr_common.la

# OPENCL engine
libfr_engine_opencl_la_SOURCES = fractalrender_engine_opencl.h fractalrender_engine_opencl.c
libfr_engine_opencl_la_CFLAGS = @OPENCL_CFLAGS@
libfr_engine_opencl_la_LDFLAGS = @OPENCL_LDFLAGS@ @OPENCL_LIBS@
libfr_engine_opencl_la_LIBADD = libfr_common.la

# bmp io lib
libfr_io_bmp_la_SOURCES = fractalrender_io_bmp.c fractalrender_io_bmp.h
libfr_io_bmp_la_LDFLAGS =
libfr_io_bmp_la_LIBADD = libfr_common.la

# png io lib
libfr_io_png_la_SOURCES = fractalrender_io_png.c fractalrender_io_png.h
libfr_io_png_la_LDFLAGS = -lpng
libfr_io_png_la_LIBADD = libfr_common.la

# interactive library
libfr_interactive_sdl_la_SOURCES = fractalrender_interactive_sdl.c fractalrender_interactive_sdl.h
libfr_interactive_sdl_la_CFLAGS = @SDL_CFLAGS@
libfr_interactive_sdl_la_LDFLAGS = @SDL_LDFLAGS@

# shared library with common sources
libfr_common_la_SOURCES = fractalrender.h fractalrender_common.h fractalrender_log.h fractalrender_types.h fractalrender_color.h fractalrender_lib.h gen_image.h gen_interactive.h  fractalrender_common.c fractalrender_log.c fractalrender_color.c fractalrender_lib.c gen_image.c gen_interactive.c
libfr_common_la_CFLAGS =
libfr_common_la_LDFLAGS =


##

fractalrender_SOURCES = fractalrender.c fractalrender_engine_c.h
# use static linking for the common library
fractalrender_LDADD = $(lib_LTLIBRARIES)
fractalrender_CFLAGS =
fractalrender_LDFLAGS =
