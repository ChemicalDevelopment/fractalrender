# FractalRender


## What is it?

A program that generates images and videos of the mandelbrot fractals


## TODO

Lots to be done!

  * Create more engines [functional: C, OpenCL, MPF, MPC] [future: CUDA]
  * Create some color formats swappable at runtime.



## Running

Once compiled, run `fractalrender -h` to view help.

This will give output like:

```
FractalRender v0.0.1

--info                                  show info
-h, --help                              show help / usage
--authors                               show authors
-e, --engine=S                          engine (C, MPF, MPC, OPENCL)
--from-raw=S                            input from .raw files
-A                                      create multiple frames
-p, --prec=N                            min bits of precision (only supprted in MPF,MPC engine)
-col, --color=S                         red color pattern
--sec=F                                 seconds
--fps=F                                 frames per second
--zps=F                                 zoom per second
-d, --dim=N,N                           dimensions of image/video
-4k                                     4k image
-i, --iter=N                            number of iterations to run
-c, --center=S,S                        x, y coordinates of middle of image
-l, --location=S                        location name, opposed to coordinates
-z, --zoom=S                            zoom level
-CLkernel=S                             OpenCL engine kernel
-CLdevice=N                             OpenCL device number
-CLsize=N,N                             OpenCL local item size
S                                       file to save as

Authors: 
  Cade Brown <cade@chemicaldevelopment.us>
```

### Example Usage

You can run `fractalrender` with no arguments, and a file named `out.png` will be created, with a good default image

Here a good example image: `fractalrender -l elephantvalley -z 100 -i 600`

The `-l` flag can be used for popular locations.

You can use positions (with `-c` for center flag) instead of the `-l` flag:

`fractalrender -c .2821 .01`

You can specify a different output here:

`fractalrender here.png`

Or, use raw formatted data, to then colorize faster (using precomputed pixels).

`fractalrender here.raw && fractalrender here.png --from-raw here.raw`

Now, you don't need to calculate the `.raw` file everytime you want to recolorize the same image.



#### Animation

For making animations, you can run the default: `fractalrender -A`. This will generate a number of PNG images, and will print out a command to run with `ffmpeg` to join them into a video.

**`-A` does not generate a video. You need [FFMPEG](http://ffmpeg.org) to string together output**.

If you want to store temporary `.png` s somewhere, you can run with the normal output, like `fractalrender /tmp/%d_out.png -A` this will replace the first `%d` with the index of the png.

You can use `--sec` for how many seconds, `--fps` for the frames per second, and `--zps` for zoom per second.

To make a 15 second video zooming in on a nice default point, run this:

`fractalrender -A --sec 15 --fps 6 --zps 1.8 -i 100 -l elephantvalley vid_%d.png`

Or, to do in two steps:

`fractalrender -A --sec 15 --fps 6 --zps 1.8 -i 100 -l elephantvalley vid_%d.raw`

Then, `fractalrender --fps 6 --from-raw vid_%d.raw vid_%d.png`

These two commands have the same effect. However, using the latter method has a main advantage: you can rerun the second command with different colorization options, and it goes much faster, because it uses precomputed pixel data. However, for large projects, .raw files can take up a lot of room. For the average user, the first method is clearer.



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

You can use `--without-X` to disable support. Thus, the minimal build configuration, only able to create `.raw` files using C code is achieved by using `./configure --without-mpi --without-gmp --without-mpfr --without-mpc --without-png --without-opencl`.




