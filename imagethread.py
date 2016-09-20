from lib import *
import numpy
import sys
import cmath
from math import *
#Runs with set paramaters and stores in arr
def run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, FRAME_NUM, FRAME_MAX, func=None):
    sys.stdout.write("\r %" + str(int(100 * (FRAME_NUM + 1) / FRAME_MAX)) + " Done  ")
    sys.stdout.flush()
    arr = numpy.zeros((DIMENSIONS[1], DIMENSIONS[0], 3), dtype=numpy.uint8)
    x = CENTER[0] - 1.0 / (ZOOM)
    px = 0
    #We use this to save time
    d = 2.0 / (ZOOM * DIMENSIONS[0])
    #Loop through x pixels
    while (px < DIMENSIONS[0]):
        #Reset y
        y = CENTER[1] + 1.0 / (ZOOM) * ((DIMENSIONS[1] + 0.0) / DIMENSIONS[0])
        py = 0
        #Loop through y pixels
        while (py < DIMENSIONS[1]):
            #Yes, it is y, x pixels
            if func != None:
                if func.lower() == "mand":
                    iterations = rawIterations_mand(x, y, ITER)
                else:
                    iterations = rawIterations(x, y, ITER, func)
            else:
                iterations = rawIterations_mand(x, y, ITER)
            arr[py, px] =  colorize(PATTERN, iterations, ITER)
            y -= d
            py += 1
        x += d
        px += 1
    return (arr, FRAME_NUM)