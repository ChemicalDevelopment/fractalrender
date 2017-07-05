/* gen_image.c -- generates images

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

void gen_image(fr_t * fr, fr_prop_lib_t * fr_prop_lib, fr_libsearch_t * libsearch, fr_engine_t * fr_engine, fr_out_t * fr_out) {
    log_debug("now in gen_image");

    struct timeval compute_stime, compute_etime, io_stime, io_etime;

    fr_io_t fr_io;

    fr_out->file_out_ext = fr_get_ext(fr_out->file_out);

    if (STR_EQIC(fr_out->file_out_ext, "mp4") || STR_EQIC(fr_out->file_out_ext, "avi")) {
        fr->anim.do_anim = true;
        fr->anim.total_frames = (int)floor(fr->anim.fps * fr->anim.sec);
        fr_out->file_write_out = (char *)malloc(strlen(fr_out->file_tmp_out) + 30);
        log_info("going to compute %d frames", fr->anim.total_frames);
        if (STR_EQIC(fr_out->file_tmp_out, "@tmp")) {
            int trand = rand() & 0xFFFF;
            struct stat st = { 0 };

            // plenty of room
            fr_out->file_tmp_out = (char *)malloc(120);
            sprintf(fr_out->file_tmp_out, "/tmp/fr_out_%04x/", trand);

            if (stat(fr_out->file_tmp_out, &st) == -1) {
                log_info("creating tmp directory %s", fr_out->file_tmp_out);
                mkdir(fr_out->file_tmp_out, 0777);
            }

            sprintf(fr_out->file_tmp_out, "%s%s", fr_out->file_tmp_out, "%08d_out.bmp");
        }

        printf(";%s;\n", fr_out->file_tmp_out);

        fr_out->file_tmp_out_ext = fr_get_ext(fr_out->file_tmp_out);

        fr_find_io(&fr_io, libsearch, fr_out->file_tmp_out_ext);

    } else {
        fr->anim.do_anim = false;

        fr->anim.total_frames = 1;
        fr_out->file_write_out = fr_out->file_out;

        if (fr_out->file_out_ext == NULL || STR_EQIC(fr_out->file_out_ext, "")) {
            log_error("unknown extension for file: %s", fr_out->file_out);
        } else {
            fr_find_io(&fr_io, libsearch, fr_out->file_out_ext);
        }
    }

    fr_out->file_out_ext = fr_get_ext(fr_out->file_out);
    fr_out->file_tmp_out_ext = fr_get_ext(fr_out->file_tmp_out);

    unsigned int total_pixels = fr->anim.total_frames * fr->dim.width * fr->dim.height;

    double compute_time = 0, io_time = 0;

    (*fr_engine->export->fr_init)(fr);

    int c_frame;

    FILE *out;

    double zoomin_fact = pow(fr->anim.zoomps, 1.0 / fr->anim.fps);

    for (c_frame = 0; c_frame < fr->anim.total_frames; ++c_frame) {
        fr->anim.c_frame = c_frame;
        fr->anim.c_time = (double)(1.0 * c_frame) / (1.0 * fr->anim.fps);

        fr_prop_lib->export->fr_zoomin(fr, zoomin_fact);

        fr_ctime(&compute_stime);

        (*fr_engine->export->fr_compute)(fr);

        fr_ctime(&compute_etime);

        compute_time += fr_diffsec(compute_etime, compute_stime);

        if (fr->anim.do_anim) {
            sprintf(fr_out->file_write_out, fr_out->file_tmp_out, c_frame + 1);
        }

        log_info("writing to file %s", fr_out->file_write_out);

        out = fr_sfopen(fr_out->file_write_out, "wb");

        fr_ctime(&io_stime);

        (*fr_io.export->fr_dump)(fr, out);

        fr_ctime(&io_etime);

        io_time += fr_diffsec(io_etime, io_stime);

        fclose(out);
    }

    if (fr->anim.do_anim) {
        char * test_ffmpeg_cmd = "ffmpeg -loglevel panic -hide_banner -nostats -h > /dev/null";
        log_info("running system command to test for ffmpeg: '%s'", test_ffmpeg_cmd);
        char * ffmpeg_cmd = (char *)malloc(100 + 100 + strlen(fr_out->file_tmp_out) + strlen(fr_out->file_write_out) + strlen(fr_out->file_out));
        sprintf(ffmpeg_cmd, "ffmpeg -loglevel panic -hide_banner -nostats -y -i %s %s -framerate %d", fr_out->file_tmp_out, fr_out->file_out, fr->anim.fps);
        bool have_ffmpeg = system(test_ffmpeg_cmd) == 0;
        if (have_ffmpeg) {
            log_info("running system command to create video: '%s'", ffmpeg_cmd);
            bool ext_code = system(ffmpeg_cmd);
            if (ext_code != 0) {
                log_warn("result was non-zero!");
            }
        } else {
            log_warn("Didn't detect ffmpeg. Run this command: '%s'", ffmpeg_cmd);
        }
    }

    printf("\n");

    log_info("compute took %lfs (%lf Mpixels/s)", compute_time, total_pixels / (1e6 * compute_time));
    log_info("io took %lfs (%lf Mpixels/s)", io_time, total_pixels / (1e6 * io_time));
    log_info("combined took %lfs (%lf Mpixels/s)", compute_time + io_time, total_pixels / (1e6 * (compute_time + io_time)));

}
