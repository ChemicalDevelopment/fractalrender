/* fractalrender_interactive_gtk.h -- interactive viewer, with gtk bindings

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

#ifndef __FRACTALRENDER_INTERACTIVE_GTK_H__
#define __FRACTALRENDER_INTERACTIVE_GTK_H__

#include <gtk/gtk.h>

void fr_interactive_gtk_init(fr_t * fr, fr_prop_lib_t * fr_prop_lib, fr_engine_t * fr_engine);

void fr_interactive_gtk_interactive(fr_t * fr, fr_prop_lib_t * fr_prop_lib, fr_engine_t * fr_engine);


#endif
