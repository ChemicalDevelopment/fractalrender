# FractalRender


## What is it?

A program that generates images and videos of the mandelbrot fractals



## Building

You need [cargs](http://chemicaldevelopment.us/cargs), and here's how to [install cargs](http://chemicaldevelopment.us/cargs/#/installing).

You also need a C compiler, and GNU Autotools. Now, run `autoreconf -i`.

You can now use `./configure && make && make install`

Use `./configure --help` for help on configuring.


## TODO

Lots to be done!

  * Create more engines [functional: C, OpenCL, MPF] [future: CUDA]



## Running

Once compiled, run `fractalrender -h` to view help.

This will give output like:

```
FractalRender v0.3.0

--info                                  show info
-h, --help                              show help / usage
--authors                               show authors
-e, --engine=S                          engine (C, MPF, OPENCL)
-col, --color=S                         color scheme (RED, BW, $FILE, etc)
-cols, --color-simple                   use simple color bands, instead of gradient
-colm, --color-mult=F                   change color period
-cold, --color-disp=F                   change color displacement
-colo, --color-out=F                    output color file
-ncs, --num-colors=N                    number of colors
--no-image                              don't create image
-p, --prec=N                            min bits of precision (only supprted in MPF engine)
--sec=F                                 seconds
--fps=F                                 frames per second
--zps=F                                 zoom per second
-d, --dim=N,N                           dimensions of image/video
-4k                                     4k image
-i, --iter=N                            number of iterations to run
-c, --center=S,S                        x, y coordinates of middle of image
-l, --location=S                        location name, opposed to coordinates
-z, --zoom=S                            zoom level
-CLdevice=S                             OpenCL device ('CPU', 'GPU', or 'ALL')
-CLdevicenum=N                          OpenCL device number
-CLkernel=S                             OpenCL engine kernel
-CLdevice=N                             OpenCL device number
-CLsize=N,N                             OpenCL local item size
--ffmpeg=S                              ffmpeg binary
--anim-tmp=S                            store temporary files for animation
S                                       file to save as

Authors: 
  Cade Brown <cade@chemicaldevelopment.us>
```

### Example Usage

You can run `fractalrender` with no arguments, and a file named `out.png` will be created, with a good default image (or, if you don't have libpng, fractalrender will create `out.bmp`)

Here a good example image: `fractalrender -l elephantvalley -z 100 -i 600`

The `-l` flag can be used for popular locations.

You can use positions (with `-c` for center flag) instead of the `-l` flag:

`fractalrender -c .2821 .01`

You can specify a different output here:

`fractalrender here.png`

Or, use raw BMP fle:

`fractalrender here.bmp`


### Colorizing

To colorize, use the `-col` option.

For example, use

`fractalrender -col RAND` to create a random colorization pattern.

It uses interpolation to create a smooth image without the normal aliased bands that are common.


#### Animation

For making animations, you can run the default: `fractalrender out.mp4`. This will generate a number of PNG images, and will use `ffmpeg` to create the result. Use `--ffmpeg` if ffmpeg is installed somewhere not on the PATH.

If ffmpeg is not installed or found, fractalrender will output a command to run to manually create the video.


If you want to store temporary `.png` s somewhere other than `/tmp/`, you can use `--anim-tmp`, like `fractalrender out.mp4 --anim-tmp %d_out.png` this will replace the first `%d` with the index of the png.

You can use `--sec` for how many seconds, `--fps` for the frames per second, and `--zps` for zoom per second.

To make a 15 second video zooming in on a nice default point, run this:

`fractalrender out.mp4 --sec 15 --fps 6 --zps 1.8 -i 100 -l elephantvalley --anim-tmp vid_%d.png`


## Compiling


### Configure Options

| Option | Description |
|:------------- |:------------- |
| `--with-cargs[=dir]` | Gives info about finding cargs in a non standard directory |
| `--with-png[=dir]` | Gives info about finding png in a non standard directory |
| `--with-gmp[=dir]` | Gives info about finding GMP in a non standard directory |
| `--with-mpfr[=dir]` | Gives info about finding GMP in a non standard directory |
| `--with-mpc[=dir]` | Gives info about finding GMP in a non standard directory |
| `--with-opencl[=dir]` | Gives info about finding OpenCL in a non standard directory |
| `--with-mpi[=dir]` | Gives info about finding MPI in a non standard directory |


Notes: 

The `dir` for arguments starting with `--with` is optional; if you leave it off, it looks in standard places to find the library, and optionally enables them if found. Use `--without-X` to disable the automatic checking

You can use `--without-X` to disable support. Thus, the minimal build configuration, only able to create `.bmp` files using C code is achieved by using `./configure --without-mpi --without-gmp --without-mpfr --without-mpc --without-png --without-opencl`.



