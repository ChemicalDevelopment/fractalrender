/* jobs.c --

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


#ifndef __JOBS_H__
#define __JOBS_H__

void figure_out_job(fractal_img_t *fractal);

#ifdef HAVE_GMP

mpf_t frtmp[4];

void do_anim_mpf(fractal_img_t *fractal);

void do_single_mpf(fractal_img_t *fractal);
#endif

void do_anim(fractal_img_t *fractal);

void do_single(fractal_img_t *fractal);


void do_anim_raw(fractal_img_t *fractal);

void do_single_raw(fractal_img_t *fractal);

#endif

