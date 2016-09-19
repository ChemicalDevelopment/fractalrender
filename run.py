import argparse
import threading
from threading import Thread
from PIL import Image
import imageio
import numpy
import copy

import imagethread
from lib import *

#Set args
parser = argparse.ArgumentParser(description='Generate fractal images with FractalRenderPy')
parser.add_argument('-d', '--dimensions', default=[800, 800], nargs=2, type=int, help='Dimensions of the image. Default is: 800 800')
parser.add_argument('-c', '--center', default=[0, 0], nargs=2, type=float, help='Center of the image. Default is 0 0')
parser.add_argument('-z', '--zoom', default=1, help='Scale. Default is 1')
parser.add_argument('-i', '--iterations', default=12, type=int,help='Iterations: default is 12')
parser.add_argument('-p', '--pattern', default='MOCHA', help='Color pattern. Default is MOCHA. Check wiki for more')
parser.add_argument('-a', '--animate', action='store_true', default=False, help="Use this flag to use an animated gif")
parser.add_argument('-s', '--seconds', default=3, type=int, help='Seconds, only matters if using -a. Default: 2')
parser.add_argument('-fps', '--framespersecond', default=8, type=int, help='Frames per second, only matters if using -a. Default: 12')
parser.add_argument('-zps', '--zoompersecond', default=4, type=float, help='Zoom per second, only matters if using -a. Default: 3.5')
parser.add_argument('-o', '--output', default="out", help='Output file. Don\'t put `.png` or any other extension. Default: out')
args = parser.parse_args()

#Pixel dimensions
DIMENSIONS = tuple(args.dimensions)
#Center of image, in real coordinates
CENTER = tuple(args.center)
#Scale ofimage
ZOOM = args.zoom
#Iterations
ITER = args.iterations
#Color pattern
PATTERN = args.pattern

#If they want an animated gif
if (args.animate):
    THREADS = []
    ARR_ARR = []
    FRAMES = args.framespersecond * args.seconds
    i = 0
    #loop through and create threads to run
    while (i < FRAMES):
        arr = numpy.zeros((DIMENSIONS[1], DIMENSIONS[0], 3), dtype=numpy.uint8)
        ARR_ARR.append(copy.copy(arr))
        t = Thread(target=imagethread.run, args=(DIMENSIONS, CENTER, copy.copy(ZOOM), copy.copy(ITER), PATTERN, ARR_ARR[i]))
        THREADS.append(copy.copy(t))
        THREADS[i].start()
        i += 1
        ZOOM = ZOOM * (args.zoompersecond ** (1.0 / (args.framespersecond)))
    #Wait for them to finish
    for t in THREADS:
        t.join()
    i = 0
    #Save to file
    imageio.mimsave(args.output + '.gif', ARR_ARR, fps=args.framespersecond)

else:
    #Run in this thread, it will be too much hassle to use threads for one image.
    arr = numpy.zeros((DIMENSIONS[1], DIMENSIONS[0], 3), dtype=numpy.uint8)
    imagethread.run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, arr)
    imageio.imsave(args.output + '.png', arr)