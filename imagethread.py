from lib import *
import numpy
import sys
import cmath
from math import *
from multiprocessing import Pool
import multiprocessing
import copy

#Runs with set paramaters and stores in arr
def run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, FRAME_NUM, FRAME_MAX, func=None, cores=4):
    arr = numpy.ndarray((DIMENSIONS[0], DIMENSIONS[1], 3), dtype=numpy.uint8)
    x = CENTER[0] - 1.0 / (ZOOM)
    px = 0
    #We use this to save time
    d = 2.0 / (ZOOM * DIMENSIONS[0])
    #Loop through x pixels
    pool = Pool()
    def threadCallback(res):
        arr[res[1]] = res[0]
    while (px < DIMENSIONS[0]):
        pool.apply_async(run_row, args=(copy.copy(x), copy.copy(px), copy.copy(d), copy.copy(DIMENSIONS), copy.copy(CENTER), copy.copy(ZOOM), copy.copy(ITER), copy.copy(PATTERN), copy.copy(FRAME_NUM), copy.copy(FRAME_MAX), copy.copy(func)), callback=threadCallback)
        x += d
        px += 1
    pool.close()
    pool.join()
    #sys.stdout.write("\r%" + str(int(100 * (FRAME_NUM + 1) / FRAME_MAX)) + " Done  ")
    #sys.stdout.flush()
    return (numpy.rot90(arr), FRAME_NUM)

def run_row(X, PX, D, DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, FRAME_NUM, FRAME_MAX, func=None):
    arr = numpy.ndarray((DIMENSIONS[1], 3), dtype=numpy.uint8)
    x = X ; px = PX ; d = D

    y = CENTER[1] + 1.0 / (ZOOM) * ((DIMENSIONS[1] + 0.0) / DIMENSIONS[0])
    py = 0
    #Loop through y pixels
    while (py < DIMENSIONS[1]):
        #Yes, it is y, x pixels
        arr[py] = colorize(PATTERN, rawIterations(x, y, ITER, func), ITER)
        y -= d
        py += 1
    return arr, PX