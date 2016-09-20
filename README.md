# FractalRender

A python program to generate images of the mandelbrot set.

Multithreaded, with .gif support!

# Usage

For a more in depth coverage, please view the $wiki

## Release



## Development build

First clone the repo and `cd` into the folder.

Now, run `python run.py`

To generate an image.

A few flags you can set add:
`-o file` saves it to `file`*.extension*.
    You don't enter `.png` or `.gif`.

`-d x y` to generate images with width `x` and height `y`

`-c x y` to set the center (middle) of the image to `x+iy`

`-z n` to set how zoomed in it is.

`-i n` to set how many iterations to compute. Should be roughly on par with zoom. 
    I am currently working on user friendly suggestion.

`-p pattern` what color pattern to use?
    Use `MOCHA` and `BLACKWHITE`

### Animation

*All flags after `-a` mean nothing if you aren't using `-a`. Make sure to use `-a` for gifs*

`-a` use it with no arguments after it to enable animated gif.
    If you are making an animated gif, use `-a`

`-s seconds` seconds of animation

`-fps frames` frames per second of animation

`-zps zoomps` zoom per second. More specifically, `CURRENT_ZOOM = BASE_ZOOM * (ZOOMPS) ** (t)`, where `t` is the time in seconds

`-t threads` number of threads to run. Default is your number of cores in your CPU, but if you want to use your computer, you should set it to less.