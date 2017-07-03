/* fractalrender_interactive.c -- interactive viewer

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

#ifndef __FRACTALRENDER_INTERACTIVE_SDL_H__
#define __FRACTALRENDER_INTERACTIVE_SDL_H__

#ifdef _MSC_VER
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif

SDL_Event cevent;

SDL_Surface *screen;
SDL_Surface *surface;

void fr_interactive_sdl_init(fr_t * fr, fr_engine_t * fr_engine);

void fr_interactive_sdl_interactive(fr_t * fr, fr_engine_t * fr_engine);


#endif

