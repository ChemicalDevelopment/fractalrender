from lib import *
import numpy
import sys
import cmath
from math import *
from multiprocessing import Pool
import multiprocessing
import copy

#Runs with set paramaters and stores in arr
def run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, FRAME_NUM, FRAME_MAX, func, cores):
    global d
    global c
    global z
    global i
    global p
    global f
    global ch
    d = DIMENSIONS
    c = CENTER
    z = ZOOM
    i = ITER
    p = PATTERN
    f = func
    ch = 2.0 / (ZOOM * DIMENSIONS[0])

    arr = numpy.zeros((DIMENSIONS[1], DIMENSIONS[0], 3), dtype=numpy.uint8)
    y = CENTER[1] + 1.0 / (ZOOM) * ((DIMENSIONS[1] + 0.0) / DIMENSIONS[0])
    py = 0
    #We use this to save time
    #Loop through x pixels
    pool = Pool(cores)
    def cb(x):
        if (x[1] >= 0 and x[1] < DIMENSIONS[1]):
            arr[x[0]] = x[1]
    while (py < DIMENSIONS[0]):
        pool.apply_async(run_row, args=(copy.copy(y), copy.copy(py)), callback=cb)
        y -= ch
        py += 1
    pool.close()
    pool.join()
    sys.stdout.write("\r%" + str(int(100 * (FRAME_NUM + 1) / FRAME_MAX)) + " Done  ")
    sys.stdout.flush()
    return (arr, FRAME_NUM)

def run_row(y, py):
    arr = numpy.zeros((d[0], 3), dtype=numpy.uint8)
    
    x = c[0] - 1.0 / (z)
    px = 0
    #Loop through y pixels
    while (px < d[0]):
        #Yes, it is y, x pixels
        arr[px] = colorize(p, rawIterations(x, y, i, f), i)
        x += ch
        px += 1
    return py, arr
