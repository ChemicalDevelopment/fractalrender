#!/usr/bin/python
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

from lib import Context

import imagethread

#Set args
parser = argparse.ArgumentParser(description='Generate fractal images with FractalRenderPy')
parser.add_argument('-d', '--dimensions', default=[800, 800], nargs=2, type=int, help='Dimensions of the image. Default is: 800 800')
parser.add_argument('-c', '--center', default=[0, 0], nargs=2, type=float, help='Center of the image. Default is 0 0')
parser.add_argument('-z', '--zoom', default=".5", help='Scale. Default is .5')
parser.add_argument('-zps', '--zoompersecond', default="2.0", help='Scale. Default is 2.0')
parser.add_argument('-i', '--iterations', default='20',help='Iterations: default is 20')
parser.add_argument('-p', '--pattern', default='MOCHA', help='Color pattern. Default is MOCHA. Check wiki for more')
parser.add_argument('-a', '--animate', action='store_true', default=False, help="Use this flag to use an animated gif")
parser.add_argument('-com', '--combine', action='store_true', default=False, help="Use this flag to combine currently computed frames.")
parser.add_argument('-cl', '--opencl', action='store_true', default=False, help="Use this flag to use OpenCL")
parser.add_argument('-s', '--seconds', default=4, type=int, help='Seconds, only matters if using -a. Default: 2')
parser.add_argument('-fps', '--framespersecond', default=8, type=int, help='Frames per second, only matters if using -a. Default: 12')
parser.add_argument('-o', '--output', help='Output file. Make sure to use correct output extension')
parser.add_argument('-t', '--threads', default=multiprocessing.cpu_count(), help='Number of thread. Default: Number of CPU cores')
parser.add_argument('-f', '--function', default=None, help='Function to compute. Default is mandelbrot set')
args = parser.parse_args()

cont = Context(tuple(args.dimensions), tuple(args.center), args.zoom, args.zoompersecond, args.iterations, args.pattern, args.function, int(args.threads), int(args.seconds), int(args.framespersecond))

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
if (args.animate):
    try:
        WRITER = imageio.get_writer(FIN, fps=int(args.framespersecond), macro_block_size=1)
    except:
        WRITER = imageio.get_writer(FIN, fps=int(args.framespersecond))


def combineFrames():
    for i in range(0, cont.frames):
        WRITER.append_data(imageio.imread("./tmp/file%d.png" % i))
    WRITER.close()

start_time = time.time()

def split_frames(max_f, n):
    max_f = int(floor(max_f / n)) * n
    base_run = "./mandelbrot.o %%d %%d %s %s %s %s %s %s %s %s %s" % (cont.frames, cont.sec, cont.dim[0], cont.dim[1], cont.iter, cont.center[0], cont.center[1], cont.zoom, cont.zps)
    ret = [base_run % (i, i + n) for i in range(0, max_f, n)]
    return ret

if (args.combine):
    combineFrames()
else:
    if args.animate:
        if args.opencl:
            frame_split = int(cont.frames / 4)
            cmds = split_frames(cont.frames, frame_split)
            procs = []
            for cmd in cmds:
                print cmd
                procs.append(subprocess.Popen(cmd, shell=True))
            for proc in procs:
                proc.wait()
        else:
            #loop through and create threads to run, only calling threads at a time
            for frame in range(0, cont.frames):
                cont.get_val(frame)
                #Create threads
                imageio.imsave("./tmp/file%d.png" % (frame), imagethread.run(cont))
                #pool.apply_async(imagethread.run, args=(copy.copy(DIMENSIONS), copy.copy(CENTER), copy.copy(ZOOM), copy.copy(int(ADJ_ITER)), copy.copy(PATTERN), copy.copy(FRAME), copy.copy(FRAMES), copy.copy(args.function)), callback=threadCallback)
            print "\n"
        combineFrames()
    else:
        cont.get_val(0)
        ret = imagethread.run(cont)
        imageio.imsave(FIN, ret, FMT)

end_time = time.time()
print("\nFile saved: " + FIN)
time_took = end_time - start_time
print("Took " + str(time_took) + "s")
print("Which is " + str(time_took / 3600) + "hr")
print("Which is " + str((time_took / 3600) / 24) + " days")
