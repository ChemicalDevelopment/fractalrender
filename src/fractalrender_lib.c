/* fractalrender_lib.h -- dynamic library loading

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


void fr_libsearch_init(fr_libsearch_t * libsearch) {
    libsearch->num_paths = 0;
    libsearch->biggest_path = 0;
    libsearch->paths = NULL;
}

void fr_libsearch_addpath(fr_libsearch_t * libsearch, char * path) {
    libsearch->num_paths += 1;
    if (libsearch->paths == NULL) {
        libsearch->paths = (char **)malloc(sizeof(char *) * libsearch->num_paths);
        assert(libsearch->paths != NULL);
    } else {
        libsearch->paths = (char **)realloc(libsearch->paths, sizeof(char *) * libsearch->num_paths);
        assert(libsearch->paths != NULL);
    }
    libsearch->paths[libsearch->num_paths - 1] = path;
    if (strlen(path) > libsearch->biggest_path) {
        libsearch->biggest_path = strlen(path);
    }
}

bool fr_find_lib(fr_lib_t * lib, fr_libsearch_t * libsearch, char * name) {
    lib->name = name;
    int i;
    void * ret_lib;

    // add 100 buffer
    char *csearch = (char *)malloc(libsearch->biggest_path + strlen(name) + strlen(FR_DFT_LIBO) + strlen(FR_DFT_LIBPREFIX) + 100);
    for (i = 0; i < libsearch->num_paths; ++i) {
        sprintf(csearch, "%s%s%s%s", libsearch->paths[i], FR_DFT_LIBPREFIX, name, FR_DFT_LIBO);
        log_debug("checking file %s", csearch);
        ret_lib = dlopen(csearch, RTLD_NOW);
        if (ret_lib == NULL) {
            log_debug("loading file %s gave error: %s", dlerror());
        } else {
            lib->_lib = ret_lib;
            log_debug("file %s worked for finding: %s", csearch, name);
            return true;
        }
    }

    return false;
}

void fr_find_interactive(fr_interactive_t * fr_interactive, fr_libsearch_t * libsearch, char * name) {
    log_info("looking for interactive library: %s", name);

    if (!fr_find_lib(&fr_interactive->lib, libsearch, name)) {
        log_error("could not find interactive library: %s", name);
    } else {
        log_info("found interactive library: %s", name);
    }

    char * export_name = (char *)malloc(100 + strlen(name));
    sprintf(export_name, "fr_interactive_%s_export", name);

    fr_interactive->export = (fr_interactive_lib_export_t *)dlsym(fr_interactive->lib._lib, export_name);

    if (fr_interactive->export == NULL) {
        log_error("couldn't find %s for interactive library %s", export_name, name);
    }
    if (fr_interactive->export->fr_init == NULL) {
        log_error("couldn't find fr_init() in %s for interactive library %s", export_name, name);
    }
    if (fr_interactive->export->fr_interactive == NULL) {
        log_error("couldn't find fr_interactive() in %s for interactive library %s", export_name, name);
    }

}


void fr_find_io(fr_io_t * fr_io, fr_libsearch_t * libsearch, char * name) {
    log_info("looking for I/O library: %s", name);

    if (!fr_find_lib(&fr_io->lib, libsearch, name)) {
        log_error("could not find I/O library: %s", name);
    } else {
        log_info("found I/O library: %s", name);
    }

    char * export_name = (char *)malloc(100 + strlen(name));
    sprintf(export_name, "fr_io_%s_export", name);


    fr_io->export = (fr_io_lib_export_t *)dlsym(fr_io->lib._lib, export_name);

    if (fr_io->export == NULL) {
        log_error("couldn't find %s for I/O library %s", export_name, name);
    }
    if (fr_io->export->fr_dump == NULL) {
        log_error("couldn't find fr_dump() in %s for I/O library %s", export_name, name);
    }

}


void fr_find_engine(fr_engine_t * fr_engine, fr_libsearch_t * libsearch, char * name) {
    log_info("looking for engine: %s", name);

    if (!fr_find_lib(&fr_engine->lib, libsearch, name)) {
        log_error("could not find engine: %s", name);
    } else {
        log_info("found engine: %s", name);
    }

    char * export_name = (char *)malloc(100 + strlen(name));
    sprintf(export_name, "fr_engine_%s_export", name);

    fr_engine->export = (fr_engine_lib_export_t *)dlsym(fr_engine->lib._lib, export_name);


    if (fr_engine->export == NULL) {
        log_error("couldn't find %s for engine library %s", export_name, name);
    }
    if (fr_engine->export->fr_init == NULL || fr_engine->export->fr_compute == NULL) {
        log_error("couldn't find required functions (fr_init() and fr_compute()) in %s for engine %s", export_name, name);
    }
}



