import argparse
import threading
#from threading import Thread
from multiprocessing import Pool
import multiprocessing
from PIL import Image
import imageio
import numpy
import copy
import os
from os import listdir
from os.path import isfile, join
import shutil

import imagethread


#Set args
parser = argparse.ArgumentParser(description='Generate fractal images with FractalRenderPy')
parser.add_argument('-d', '--dimensions', default=[800, 800], nargs=2, type=int, help='Dimensions of the image. Default is: 800 800')
parser.add_argument('-c', '--center', default=[0, 0], nargs=2, type=float, help='Center of the image. Default is 0 0')
parser.add_argument('-z', '--zoom', default=1, help='Scale. Default is 1')
parser.add_argument('-i', '--iterations', default=12, type=int,help='Iterations: default is 12')
parser.add_argument('-p', '--pattern', default='MOCHA', help='Color pattern. Default is MOCHA. Check wiki for more')
parser.add_argument('-a', '--animate', action='store_true', default=False, help="Use this flag to use an animated gif")
parser.add_argument('-s', '--seconds', default=4, type=int, help='Seconds, only matters if using -a. Default: 2')
parser.add_argument('-fps', '--framespersecond', default=8, type=int, help='Frames per second, only matters if using -a. Default: 12')
parser.add_argument('-zps', '--zoompersecond', default=2, type=float, help='Zoom per second, only matters if using -a. Default: 3.5')
parser.add_argument('-o', '--output', default="out", help='Output file. Don\'t put `.png` or any other extension. Default: out')
parser.add_argument('-t', '--threads', default=multiprocessing.cpu_count(), help='Number of thread. Default: Number of CPU cores')
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

#If they want an animated gif, run in lots of threads to maximize resources
if (args.animate):
    FRAMES = args.framespersecond * args.seconds
    ARR_ARR = [None] * FRAMES
    THREADS = int(args.threads)
    if os.path.exists("./tmp/"):
        shutil.rmtree('./tmp/')
    os.makedirs("./tmp/")
    #import sys
    #sys.exit()
    def threadCallback(res):
        imageio.imsave("./tmp/" + str(res[1]) + ".png", res[0], 'PNG')
    pool = Pool(THREADS)
    ADJ_ZOOM = ZOOM
    ADJ_ITER = ITER
    #loop through and create threads to run
    for i in range(0, FRAMES, THREADS):
        for j in range(0, THREADS):
            pool.apply_async(imagethread.run, args=(copy.copy(DIMENSIONS), copy.copy(CENTER), copy.copy(ADJ_ZOOM), copy.copy(ADJ_ITER), copy.copy(PATTERN), copy.copy(i + j)), callback=threadCallback)
            ADJ_ZOOM = ADJ_ZOOM * (args.zoompersecond ** (1.0 / args.framespersecond))
    pool.close()
    pool.join()
    IMAGES = [None] * FRAMES
    #imageio.mimsave(args.output + '.gif', ARR_ARR, float(fps=args.framespersecond))
    #kargs = { 'fps': 15 }
    #kargs = { 'duration': .1 }
    #imageio.mimsave(args.output + '.gif', ARR_ARR, 'GIF', **kargs)
    for f in listdir("./tmp/"):
        if isfile(join("./tmp/", f)):
            framenum = int(''.join([c for c in f if c in '1234567890']))
            IMAGES[framenum] = imageio.imread("./tmp/" + str(f))

    imageio.mimsave(args.output + ".gif", IMAGES, format='GIF', fps=args.framespersecond)
    print("Done")

else:
    #Run in this thread, it will be too much hassle to use threads for one image.
    res = imagethread.run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, 0)
    imageio.imsave(args.output + '.png', res[0])