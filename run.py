import argparse
import threading
import subprocess
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
from math import *
import time

import imagethread

#Set args
parser = argparse.ArgumentParser(description='Generate fractal images with FractalRenderPy')
parser.add_argument('-d', '--dimensions', default=[800, 800], nargs=2, type=int, help='Dimensions of the image. Default is: 800 800')
parser.add_argument('-c', '--center', default=[0, 0], nargs=2, type=float, help='Center of the image. Default is 0 0')
parser.add_argument('-z', '--zoom', default=1.0, help='Scale. Default is 1')
parser.add_argument('-i', '--iterations', default='12',help='Iterations: default is 12')
parser.add_argument('-p', '--pattern', default='MOCHA', help='Color pattern. Default is MOCHA. Check wiki for more')
parser.add_argument('-a', '--animate', action='store_true', default=False, help="Use this flag to use an animated gif")
parser.add_argument('-com', '--combine', action='store_true', default=False, help="Use this flag to combine currently computed frames.")
parser.add_argument('-cl', '--opencl', action='store_true', default=False, help="Use this flag to use OpenCL")
parser.add_argument('-s', '--seconds', default=4, type=int, help='Seconds, only matters if using -a. Default: 2')
parser.add_argument('-fps', '--framespersecond', default=8, type=int, help='Frames per second, only matters if using -a. Default: 12')
parser.add_argument('-zps', '--zoompersecond', default=2, type=float, help='Zoom per second, only matters if using -a. Default: 3.5')
parser.add_argument('-o', '--output', help='Output file. Make sure to use correct output extension')
parser.add_argument('-t', '--threads', default=multiprocessing.cpu_count() - 1, help='Number of thread. Default: Number of CPU cores')
parser.add_argument('-f', '--function', default=None, help='Function to compute. Default is mandelbrot set')
args = parser.parse_args()


#Pixel dimensions
DIMENSIONS = tuple(args.dimensions)
#Center of image, in real coordinates
CENTER = tuple(args.center)
#Scale ofimage
ZOOM = float(args.zoom)
#Iterations
ITER = args.iterations
#Color pattern
PATTERN = args.pattern
#Num frames
FRAMES = int(args.framespersecond * args.seconds)
#Base zoom
BASE_ZOOM = ZOOM + 0.0
THREADS = int(args.threads)


#Set if output is empty
if not args.output:
    if (args.animate):
        args.output = "out.mp4"
    else:
        args.output = "out.png"

FI = args.output.split(".")[0]
FMT = args.output.split(".")[1]
FIN = FI + "." + FMT

try:
    os.remove(FIN)
except:
    pass

WRITER = imageio.get_writer(FIN, fps=int(args.framespersecond))

def combineFrames():
    for i in range(0, FRAMES):
        WRITER.append_data(imageio.imread("./tmp/file" + str(i) + ".png"))
    WRITER.close()

start_time = time.time()

if (args.combine):
    combineFrames()
else:
    if args.animate:
        if args.opencl:
            commandA = ["./mandelbrot.o", args.seconds, FRAMES, DIMENSIONS[0], DIMENSIONS[1], ITER, CENTER[0], CENTER[1], ZOOM, args.zoompersecond]
            commandA = map(str, commandA)
            command = " ".join(commandA)
            print command
            process = subprocess.Popen(command, shell=True)
            process.wait()
        else:
            def threadCallback(res):
                imageio.imsave("./output/tmp/file" + str(res[1]) + ".png", res[0])
            #This limits the number of executed threads
            pool = Pool(THREADS)
            #loop through and create threads to run, only calling threads at a time
            for FRAME in range(0, FRAMES):
                TIME = (FRAME + 0.0) / args.framespersecond
                #Adjust the ZOOM
                ZOOM = BASE_ZOOM * (args.zoompersecond ** TIME)
                #Evaluate iteration
                ADJ_ITER = eval(ITER)
                #Create threads
                pool.apply_async(imagethread.run, args=(copy.copy(DIMENSIONS), copy.copy(CENTER), copy.copy(ZOOM), copy.copy(int(ADJ_ITER)), copy.copy(PATTERN), copy.copy(FRAME), copy.copy(FRAMES), copy.copy(args.function)), callback=threadCallback)
            #Close and wait
            pool.close()
            pool.join()
        combineFrames()
    else:
        imagethread.run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, 0)

end_time = time.time()
print("\nFile saved: " + FIN)
time_took = end_time - start_time
print("Took " + str(time_took) + "s")
print("Which is " + str(time_took / 3600) + "hr")
print("Which is " + str((time_took / 3600) / 24) + " days")