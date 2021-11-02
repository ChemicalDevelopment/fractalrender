# fractalrender

fractalrender is a utility to generate visuals of fractals of the [Mandelbrot fractal](https://en.wikipedia.org/wiki/Mandelbrot_set)

## Running

You can run the program via `./fr`. you can run with `-h` or `--help` to give the help menu:

```shell
usage: ./fractalrender -oOUT [-sWxH] [-cR+Ii] [-zZOOM] [name=val ...]
    -o OUT            Can be .png, .bmp, or .jpg file
    -s WxH            Size (in pixels) width by height
    -c R+Ii           Center of the image in the complex plane
    -z ZOOM           Zoom of the image in the complex plane (higher zoom is more zoomed in)
    name=val          A key (string) and value (scalar) pair, which is passed along to the fractal. Common examples are `iter=100` `colscale=2.5`, etc

fractalrender (C++ version), written by Cade Brown <me@cade.site>
```

See `EXAMPLES.md` for example images that you can generate that look cool! (and tweak from there)


## Building

There are no requirements beyond `make` and a C++ compiler. Simply run `make` in the main directory to build `./fractalrender`, the output executable

## Common Keys

The keys (`name=val` pairs) are up to each engine (some engines may or may not support any of these):

  * `iter=100`: Gives the number of iterations to render the fractal
  * `color=01295F,BDD5EA`: Gives the color scheme, as a list of colors. Each color is expected to be in HEX RGB (Red, Green, Blue components) and colors are seperated by commas
  * `colorscale=0.25`: Multiplies the color scale before sampling. Higher values mean more rings of color, lower values mean more gradual changes
  * `coloroffset=0.0`: Adds a constant offset to the color scale before sampling. Basically shifts the color scheme
  * `bsr=0`: Binary splitting on real component, which controls the tree-like effect. Use `=0` for off, `=1` for on
  * `bsi=0`: Binary splitting on imaginary component, which controls the tree-like effect. Use `=0` for off, `=1` for on
