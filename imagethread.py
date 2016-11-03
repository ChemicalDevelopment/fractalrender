from lib import *
import numpy
import sys
import cmath
from math import *
from multiprocessing import Pool
import multiprocessing
import copy

d_arr = 0#numpy.zeros((d[0], 3), dtype=numpy.uint8)

#Runs with set paramaters and stores in arr
def run(_context):
    global context
    context = _context

    arr = numpy.zeros((context.dim[1], context.dim[0], 3), dtype=numpy.uint8)

    global d_arr
    d_arr = numpy.zeros((context.dim[0], 3), dtype=numpy.uint8)

    y = context.start_y
    py = 0
    #We use this to save time
    #Loop through x pixels
    pool = Pool(context.threads)
    def cb(rr):
        arr[rr[0]] = rr[1]

    while (py < context.dim[1]):
        pool.apply_async(run_row, args=(copy.copy(y), copy.copy(py)), callback=cb)
        y -= context.change
        py += 1

    pool.close()
    pool.join()
    return arr

def run_row(y, py):
    global d_arr
    global context
    c_arr = copy.copy(d_arr)
    x = context.start_x
    px = 0
    while (px < context.dim[0]):
        c_arr[px] = get_color(context, x, y)
        x += context.change
        px += 1
    return (py, c_arr)
