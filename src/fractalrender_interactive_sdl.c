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

#include "fractalrender_interactive_sdl.h"


fr_interactive_lib_export_t fr_interactive_sdl_export = {
    fr_interactive_sdl_init,
    fr_interactive_sdl_interactive
};


SDL_Event cevent;

SDL_Surface *screen;
SDL_Surface *surface;

void fr_interactive_sdl_init(fr_t * fr, fr_engine_t * fr_engine) {
    log_debug("SDL interactive init");
  
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("SDL failed to init: %s", SDL_GetError());
    }

    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(fr->dim.width, fr->dim.height, 0, 0);
    if (screen == NULL) {
        log_error("SDL failed to create screen: %s", SDL_GetError());
    }

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, fr->dim.width, fr->dim.height, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
    if (surface == NULL) {
        log_error("SDL failed to create surface: %s", SDL_GetError());
    }

    if (fr->bitmap != NULL) {
        free(fr->bitmap);
    }

    fr->bitmap = surface->pixels;
    fr->dim.mem_width = surface->pitch;

    (*fr_engine->export->fr_init)(fr);


}

void _fr_interactive_sdl_recompute(fr_t * fr, fr_engine_t * fr_engine) {
    (*fr_engine->export->fr_compute)(fr);

    SDL_BlitSurface(surface, NULL, screen, NULL);
    SDL_Flip(screen);

}

void fr_interactive_sdl_interactive(fr_t * fr, fr_engine_t * fr_engine) {
    log_debug("SDL interactive starting");

    _fr_interactive_sdl_recompute(fr, fr_engine);

    bool keep_going = true;
    bool do_update = false;

    double zoomin_fact = 1.5;

    while (keep_going == true) {
        while (SDL_PollEvent(&cevent)) {
            switch (cevent.type) {
                case SDL_QUIT:
                    log_debug("SDL quit event");
                    keep_going = false;
                    break;
                case SDL_KEYDOWN:
                    if (cevent.key.keysym.sym == ' ') {
                        fr_set_prop(fr, "zoom", NULL, fr->prop.zoom * 1.5);
                        do_update = true;
                    } else if (cevent.key.keysym.sym == SDLK_LSHIFT || cevent.key.keysym.sym == SDLK_RSHIFT) {
                       fr_set_prop(fr, "zoom", NULL, fr->prop.zoom / 1.5);
                        do_update = true;
                    } else if (cevent.key.keysym.sym == SDLK_LEFT) {
                        fr_set_prop(fr, "center_x", NULL, fr->prop.center_x - .1 / fr->prop.zoom);
                        do_update = true;
                    } else if (cevent.key.keysym.sym == SDLK_RIGHT) {
                        fr_set_prop(fr, "center_x", NULL, fr->prop.center_x + .1 / fr->prop.zoom);
                        do_update = true;
                    } else if (cevent.key.keysym.sym == SDLK_UP) {
                        fr_set_prop(fr, "center_y", NULL, fr->prop.center_y + .1 / fr->prop.zoom);
                        do_update = true;
                    } else if (cevent.key.keysym.sym == SDLK_DOWN) {
                        fr_set_prop(fr, "center_y", NULL, fr->prop.center_y - .1 / fr->prop.zoom);
                        do_update = true;
                    } else if (cevent.key.keysym.sym == SDLK_ESCAPE) {
                        keep_going = false;
                    }
                    break;
                    /*
                case SDL_MOUSEBUTTONDOWN:
                    //fr_set_prop(fr, "center_x", NULL, );
                    center = creal(center) + ((event.button.x - (WIDTH/2))/zoom) +
                            ((cimag(center) + ((event.button.y - (HEIGHT/2))/zoom))
                              *_Complex_I);

                    if (event.button.button == 1)
                        zoom *= ZOOM_FACTOR;
                    else if (event.button.button == 3)
                        zoom /= ZOOM_FACTOR;

                    sdl_draw_mandelbrot(surface, center, zoom);
                    break;
                    */
                default:
                    //
                    break;
            }
        }
        if (!keep_going) {
            break;
        }
        if (do_update) {
            log_trace("recomputing fractal");
            _fr_interactive_sdl_recompute(fr, fr_engine);
        }
        do_update = false;
    }

    log_trace("SDL interactive main loop ended");

    SDL_Delay(500);

    SDL_Quit();
    log_debug("SDL interactive ended");

    return;
}



