# Quick Start

First, [install](./#/installing) FractalRender


## Example usages

`fractalrender --help` -- prints help message, with usage information. Run this for more info

`fractalrender` -- creates a file `out.png` with a nice default fractal image (anytime there is no out image, the output is `out.png`, except for animations)

`fractalrender myimage.png` -- creates a file `myimage.png` with a nice default fractal image

`fractalrender -4k -e OPENCL` -- creates `out.png` , a 4k image, using the OpenCL kernel to speed up calculations. You will be given an error if not compiled with OpenCL support.

`fractalrender myimage.raw` -- creates a file `myimage.raw` which is just precomputed pixel iterations, NOT an image

`fractalrender myimage.png --from-raw myimage.raw` -- creates file `myimage.png` from a raw precomputed file `myimage.raw`

`fractalrender -i 1000` -- computes the default fractal, but with an increased iteration count

`fractalrender -c .2821 .01 -z 100 -i 200` -- computes an image centered at `.2821 + .01i`, zoomed in `100x`, and with `200` iterations

`fractalrender -l elephantvalley` -- computes an image centered at a special location

`fractalrender -d 256 256` -- computes default fractal as a `256x256` image

`fractalrender -4k` -- sets the dimensions to 4k (`3840x2160`)

### Animation

With animations, the default output is `%d_out.png`, where `%d` is replaced by the frame number. So, you will have `1_out.png`, `2_out.png`, `3_out.png`, ... `NN_out.png`.

`fractalrender -A` -- creates the default animation, and prints an `ffmpeg` command to string together the `.png` files to create a video

`fractalrender -A %d_out.raw` -- precomputes the default animation into `.raw` files, which can be later made into `.png` s

`fractalrender -A %d_out.png --from-raw %d_out.raw` -- creates `.png` files from `.raw` files, and prints out an `ffmpeg` command to create a video.

`fractalrender -A -c .2821 .01 -z .25 --zps 3.0 --fps 8 -i 600 --sec 7 -d 640 480` -- creates an animation centered at `.2821 + .01i`, beginning at zoom level `.25`, multiplying the zoom by `3.0` each second, running at `8` frames per second, using `600` iterations, and lasting for `7` seconds





