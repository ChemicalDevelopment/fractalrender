
# Installing

## Dependencies/Requirements

### Mandatory

You need a C compiler and [cargs](http://chemicaldevelopment.us/cargs)

### Optional

You can optionally install the following software, each with added purpose:

[libpng](http://www.libpng.org/pub/png/libpng.html), for direct to .png support [most people will already have this].

[GMP](https://gmplib.org/), for high-precision support beyond 64 bits

[MPFR](https://mpfr.org/), required for MPC

[MPC](https://http://www.multiprecision.org/), full complex number suite, and (possibly in the future), custom kernels

OpenCL, for a very accelerated kernel compute (expect 10x speedups on `.raw` creation).

[MPI](https://www.open-mpi.org/), for distributed multithreading animations.


If you have these installed already in standard locations, the configure step will autodetect them.

# Configuring

This project uses GNU autotools for source, so when you recieve your tarball, run:

`./configure && make && make install`

to build the software.

The `./configure` script has a number of options, such as:

| Option | Description |
|:------------- |:------------- |
| `--with-[cargs\|gmp\|mpfr\|mpc\|mpi\|opencl\|png]=DIR` | Gives info about finding a library in a non standard directory |


If you don't specify any, FractalRender will try to check if you have each piece, and if found, include and utilize it.


So, to compile with a local copy of `libpng`, run `./configure --with-png=$HOME/prefix && make && make install`



