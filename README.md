# FractalRender


## What is it?

A program that generates images and videos of the mandelbrot fractals


## TODO

Lots to be done!

  * Create more engines [started: C] [future: OpenCL, GMP_C, MPFR_C, MPC_C, CUDA]
  * Create more precisions for C engine to handle without GMP/etc (emulating 256 bit, and possibly up to 2048 or so).
  * Create some color formats swappable at runtime.
  * Find out why and bfix bit depths > 8 being buggy
  * Add videos, gifs, and other output formats (including raw results to read back at another time)




## Running

Once compiled, run `fractalrender -h` to view help.

This will give output like:

```
FractalRender v0.0.1

--info                                  show info
-h, --help                              show help / usage
--authors                               show authors
-4k                                     4k image
-d, --dim=N,N                           dimensions of image/video
-p, --prec=N                            min bits of precision
-e, --engine=S                          engine (C, GMP, ...)
-col, --Rcolor=S                        red color pattern
-Rcol, --Rcolor=S                       red color pattern
-Gcol, --Gcolor=S                       green color pattern
-Bcol, --Bcolor=S                       blue color pattern
-i, --iter=N                            number of iterations to run
-l, --location=S,S                      location name, opposed to coordinates
-c, --center=S,S                        x, y coordinates of middle of image
-z, --zoom=S                            zoom level
-CLkernel=S                             OpenCL engine kernel
S                                       file to save as

Authors: 
  Cade Brown <cade@chemicaldevelopment.us>

```


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

The `dir` for arguments starting with `--with` is optional; if you leave it off, it looks in standard places to find the library, and optionally enables them if found.

`--enable-static` is known to not work with macOS




