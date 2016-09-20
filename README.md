# FractalRender

A python program to generate images of the mandelbrot set.

Multithreaded, animated with support for:
`.png`, `.jpg`, `.mov`, `.avi`, `.mp4`, `.bmp` and more!

# Usage

For a more in depth coverage, please view the $wiki

## Release

Coming soon!


## Development build

First clone the repo and `cd` into the folder.

Now, run `python run.py`

To generate an image.

You can change properties using these flags:

### Flags
`-o file` saves it to `file`, and uses correct format for any image format.
    You can use `.gif`, `.avi`, `.mp4` or most other formats. 
    Example `-o test.gif`

`-d x y` to generate images with width `x` and height `y`
    Example: `-d 1920 1080`

`-c x y` to set the center (middle) of the image to `x+iy`
    Example: `-c .2821 .01`

`-z n` to set how zoomed in it is.
    Example: `-z 4.5123`

`-i n` to set how many iterations to compute. Should be roughly on par with zoom. 
    Example: `-i 50`

`-p pattern` what color pattern to use?
    Example: `-p MOCHA` Find more on the wiki

#### Animation

*All flags after `-a` mean nothing if you aren't using `-a`. Make sure to use `-a` for gifs*

`-a` use it with no arguments after it to enable animation. You can use `.gif`, `.mov`, `.mp4` and almost any other movie format
    Example: `-a`

`-s seconds` seconds of animation
    Example: `-s 3.5`

`-fps frames` frames per second of animation
    Example: `-fps 18`

`-zps zoomps` zoom per second. More specifically, `CURRENT_ZOOM = BASE_ZOOM * (ZOOMPS) ** (t)`, where `t` is the time in seconds
    Example: `-zps 2.3212`

`-t threads` number of threads to run. Default is your number of cores in your CPU, but if you want to use your computer, you should set it to less.
    Example: `-t 2`

#### Examples
    `python run.py -d 1920 1080 -c 0 0 -z 0.25 -o wholeimage.png -i 12 -p MOCHA`
        Generates a low iteration count render of the whole mandelbrot set