/* fractalrender_engine_cuda.h -- CUDA engine

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

#include "fractalrender_engine_cuda.h"


fr_engine_lib_export_t fr_engine_cuda_export = {
    fr_engine_cuda_init,
    fr_engine_cuda_compute
};

cuda_fr_t * cuda_fr;


void engine_cuda_print_help() {
    printf("Cuda Engine Help\n");
    printf("  -n             x dimension\n");
    printf("  -m             y dimension\n");
    printf("  -h             print help\n");
    printf("\n");
}

void fr_engine_cuda_init(fr_t * fr) {
    cuda_fr = (cuda_fr_t *)malloc(sizeof(cuda_fr_t));
    mand_cuda_init(cuda_fr, fr);

    cuda_fr->dimx = 4;
    cuda_fr->dimy = 4;

    char c;

    while ((c = getopt (fr->argc, fr->argv, "n:m:h")) != optstop) {
        switch (c) {
            case 'n':
                cuda_fr->dimx = atoi(optarg);
                break;
            case 'm':
                cuda_fr->dimy = atoi(optarg);
                break;
            case 'h':
                engine_cuda_print_help();
                FR_FINISH
                break;
            case '?':
                engine_cuda_print_help();
                break;
            default:
                log_fatal("unknown getopt");
                break;
        }
    }

}

void fr_engine_cuda_compute(fr_t * fr) {
    cuda_fr->fr = fr;
    
    mand_cuda(cuda_fr);
}
