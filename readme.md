# fractalrender

fractalrender is a utility to generate visuals of fractals of the [Mandelbrot fractal](https://en.wikipedia.org/wiki/Mandelbrot_set). 

## Install

TODO: make installable

## Build

Building fractalrender requires a C++ compiler and `make`. If you have those dependencies, building is as simple as:

```shell
$ make
```

## Usage

You can run `fractalrender` (or `./fractalrender` if built locally) with `--help` to print usage information:

```shell
$ ./fractalrender --help
```


## Common Keys

The keys (`name=val` pairs) are up to each engine (some engines may or may not support any of these):

  * `iter=100`: Gives the number of iterations to render the fractal
  * `color=01295F,BDD5EA`: Gives the color scheme, as a list of colors. Each color is expected to be in HEX RGB (Red, Green, Blue components) and colors are seperated by commas
  * `colorscale=0.25`: Multiplies the color scale before sampling. Higher values mean more rings of color, lower values mean more gradual changes
  * `coloroffset=0.0`: Adds a constant offset to the color scale before sampling. Basically shifts the color scheme
  * `bsr=0`: Binary splitting on real component, which controls the tree-like effect. Use `=0` for off, `=1` for on
  * `bsi=0`: Binary splitting on imaginary component, which controls the tree-like effect. Use `=0` for off, `=1` for on
