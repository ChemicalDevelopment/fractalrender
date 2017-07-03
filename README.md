# fractalrender

fractalrender is a program that can render images and videos of the mandelbrot fractal, julia fractals, and more. It also includes a framework for people to write their own engines to generate images of fractals, supported dynamically at runtime.

It will also (still WIP) support an interactive viewer (using SDL).

fractalrender also plans to support Windows.


## Usage

```
fractalrender v0.4.0
Usage: fractalrender [options] -o file...
Options:
  --help                     show help and exit
  -w <width>                 set image width to <width>
  -h <height>                set image width to <height>
  -i <iter>                  set max iterations to <iter>
  -z <zoom>                  set image zoom to <zoom>
  -x <center x>              set image center to <center x>
  -y <center y>              set image center to <center y>
        the center of the image is x + iy

  -c <color>                 set color scheme to <color>
        if <color> is a file that ends with .color, the color scheme is read in.
        Otherwise, some builtins, such as `red`, `green`, `mocha`, and `random` can be used.

  -e <engine>                set compute engine to <engine>
        This looks for libraries. If this is failing, try using `-e c` for the default engine.

  -o <file>                  save image to <file>
        the extension determines which color I/O library to load. 
        If your library is not loading, try using `.bmp` files

  -s                         do simple colorizing

To report bugs, please email:
<group@chemicaldevelopment.us>
```


## Common Problems


 * P: When you run `fractalrender` after installation, it fails with: `fractalrender: error while loading shared libraries: libfrcommon.so.0: cannot open shared object file: No such file or directory`. S: run `sudo ldconfig`, or run locally like: `./src/fractalrender`





