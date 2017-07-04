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

lib_LTLIBRARIES = libfrcommon.la libfrbmp.la libfrc.la libfrcomplex.la

if HAVE_PNG
lib_LTLIBRARIES += libfrpng.la
endif

if HAVE_SDL
lib_LTLIBRARIES += libfrsdl.la
endif

bin_PROGRAMS = fractalrender


###

# C engine
libfrc_la_SOURCES = fractalrender_engine_c.h fractalrender_engine_c.c
libfrc_la_LDFLAGS = 
libfrc_la_LIBADD = libfrcommon.la 


# C complex engine
libfrcomplex_la_SOURCES = fractalrender_engine_complex.h fractalrender_engine_complex.c
libfrcomplex_la_LDFLAGS =
libfrcomplex_la_LIBADD = libfrcommon.la

# bmp io lib
libfrbmp_la_SOURCES = fractalrender_io_bmp.c fractalrender_io_bmp.h
libfrbmp_la_LDFLAGS =
libfrbmp_la_LIBADD = libfrcommon.la

# png io lib
libfrpng_la_SOURCES = fractalrender_io_png.c fractalrender_io_png.h
libfrpng_la_LDFLAGS = -lpng
libfrpng_la_LIBADD = libfrcommon.la

# interactive library
libfrsdl_la_SOURCES = fractalrender_interactive_sdl.c fractalrender_interactive_sdl.h
libfrsdl_la_CFLAGS = @SDL_CFLAGS@
libfrsdl_la_LDFLAGS = @SDL_LDFLAGS@

# shared library with common sources
libfrcommon_la_SOURCES = fractalrender.h fractalrender_common.h fractalrender_log.h fractalrender_types.h fractalrender_color.h fractalrender_lib.h gen_image.h gen_interactive.h  fractalrender_common.c fractalrender_log.c fractalrender_color.c fractalrender_lib.c gen_image.c gen_interactive.c
libfrcommon_la_CFLAGS = 
libfrcommon_la_LDFLAGS =


##

fractalrender_SOURCES = fractalrender.c fractalrender_engine_c.h
# use static linking for the common library
fractalrender_LDADD = $(lib_LTLIBRARIES)
fractalrender_CFLAGS = 
fractalrender_LDFLAGS = 

