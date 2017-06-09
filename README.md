# FractalRender


## What is it?

A program that generates images and videos of the mandelbrot fractals

## Running

Once compiled, run `fractalrender -h` to view help.

This will give output like:

```
FractalRender v0.0.1

--info                                  show info
-h, --help                              show help / usage
--authors                               show authors
-d, --dim=N,N                           dimensions of image/video
-bd, --bit-depth=N                      bit depth of tests
-i, --iter=N                            number of iterations to run
-c, --center=S,S                        x, y coordinates of middle of image
-z, --zoom=S                            zoom level
S                                       file to save as

Authors: 
  Cade Brown <cade@chemicaldevelopment.us>

```


## Compiling


### Configure Options

| Option | Description |
|:------------- |:------------- |
| `--enable-static` | Builds a static executable (for distributing) |
| `--with-cargs[=dir]` | Gives info about finding cargs in a non standard directory |
| `--with-png[=dir]` | Gives info about finding png in a non standard directory |
| `--with-gmp[=dir]` | Gives info about finding GMP in a non standard directory |
| `--with-opencl[=dir]` | Gives info about finding OpenCL in a non standard directory |
| `--with-mpi[=dir]` | Gives info about finding MPI in a non standard directory |


Notes: 

The `dir` for arguments starting with `--with` is optional; if you leave it off, it looks in standard places to find the library.

`--enable-static` is known to not work with macOS






