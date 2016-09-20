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

FI = args.output.split(".")[0]
FMT = args.output.split(".")[1]
FIN = FI + "." + FMT

#If they want an animated gif, run in lots of threads to maximize resources
if (args.animate):
    #How many frames?
    FRAMES = int(args.framespersecond * args.seconds)
    ARR_ARR = [None] * FRAMES
    THREADS = int(args.threads)
    def threadCallback(res):
        ARR_ARR[res[1]] = res[0]
    #This limits the number of executed threads
    pool = Pool(THREADS)
    ADJ_ZOOM = float(ZOOM)
    ADJ_ITER = ITER
    #loop through and create threads to run, only calling threads at a time
    for i in range(0, FRAMES, THREADS):
        #Create threads
        for j in range(0, THREADS):
            #Start a thread in the pool.
            pool.apply_async(imagethread.run, args=(copy.copy(DIMENSIONS), copy.copy(CENTER), copy.copy(ADJ_ZOOM), copy.copy(int(ADJ_ITER)), copy.copy(PATTERN), copy.copy(i + j), FRAMES), callback=threadCallback)
            #Adjust the ZOOM
            ADJ_ZOOM = ADJ_ZOOM * (args.zoompersecond ** (1.0 / args.framespersecond))
    #Close and wait
    pool.close()
    pool.join()
    #Save to file
    imageio.mimsave(FIN, ARR_ARR, format=FMT, fps=args.framespersecond)
else:
    #Run in this thread, it will be too much hassle to use threads for one image.
    res = imagethread.run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, 0, 1)
    imageio.imsave(FIN, res[0], format=FMT)
print("\nFile saved: " + FIN)
