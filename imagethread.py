from lib import *
import numpy

#Runs with set paramaters and stores in arr
def run(DIMENSIONS, CENTER, ZOOM, ITER, PATTERN, FRAME_NUM):
    print("Running frame: " + str(FRAME_NUM))
    arr = numpy.zeros((DIMENSIONS[1], DIMENSIONS[0], 3), dtype=numpy.uint8)
    x = CENTER[0] - 1.0 / (ZOOM)
    px = 0
    #We use this to save time
    d = 2.0 / (ZOOM * DIMENSIONS[0])
    #Loop through x pixels
    while (px < DIMENSIONS[0]):
        #Reset y
        y = -CENTER[1] - 1.0 / (ZOOM)
        py = 0
        #Loop through y pixels
        while (py < DIMENSIONS[1]):
            #Yes, it is y, x pixels
            arr[py, px] = colorize(PATTERN, rawIterations(x, y, ITER), ITER)
            y += d
            py += 1
        x += d
        px += 1
    return (arr, FRAME_NUM)