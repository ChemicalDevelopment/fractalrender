/* fractalrender.h -- header file for fractalrender

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

#ifndef __FRACTALRENDER_H__
#define __FRACTALRENDER_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dlfcn.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FR_DFT_WIDTH             1920
#define FR_DFT_HEIGHT            1080
#define FR_DFT_MAXITER             50
#define FR_DFT_ER                 4.0
#define FR_DFT_CENTERX          "0.0"
#define FR_DFT_CENTERY          "0.0"
#define FR_DFT_ZOOM             "0.5"
#define FR_DFT_COLORSCHEME      "red"
#define FR_DFT_O            "out.bmp"
#define FR_DFT_ENGINE             "c"
#define FR_DFT_ANIMTMP         "@TMP"
#define FR_DFT_ANIMFPS             10
#define FR_DFT_ANIMSEC           10.0
#define FR_DFT_ANIMZPS          "1.9"
#define FR_DFT_LOGLVL        LOG_INFO

#define FR_DFT_LIBPREFIX      "libfr"

#define FR_BYTEDEPTH                4

// static object out, maybe dll on windows
#ifdef _WIN32
#define FR_DFT_LIBO            ".dll"
#else
#define FR_DFT_LIBO             ".so"
#endif

#include "fractalrender_log.h"

#define FR_FAIL exit(3);


#include "fractalrender_types.h"

#include "fractalrender_lib.h"

#include "fractalrender_common.h"

#include "fractalrender_color.h"

#include "gen_image.h"

#include "gen_interactive.h"


#endif


