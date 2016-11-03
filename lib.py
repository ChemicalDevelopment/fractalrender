#Currently testing
try:
    from mpmath import *
except:
    print "mpmath not found, going along as normal"

import random

def get_color(pattern, x, y, maxIterations, func):
    return colorize(pattern, rawIterations(x, y, maxIterations, func), maxIterations)

#Returns the iterations of (x+iy). Returns 0 through maxIterations inclusive
def rawIterations_mand(x, y, maxIterations):
    re = x
    im = y
    iter = 0
    while re * re + im * im < 4 and iter < maxIterations:
        tmp = 2 * re * im
        re = re * re - im * im + x
        im = tmp + y
        iter += 1
    return iter

#Returns the iterations of (x+iy). Returns 0 through maxIterations inclusive
def rawIterations(x, y, maxIterations, func):
    if not func or func == "z**2+c":
        return rawIterations_mand(x, y, maxIterations)
    z = 0 + 0j
    c = x + y*1j
    iter = 0
    while z.imag * z.imag + z.real * z.real <= 4 and iter < maxIterations:
        try:
            z = eval(func)
        except:
            z = c
        #tmp = 2 * re * im
        #re = re * re - im * im + x
        #im = tmp + y
        iter += 1
    return iter

#Colorizes based on pattern
def colorize(pattern, iter, maxIterations):
    pattern = pattern.upper()
    if pattern == "MOCHA":
        piter = .6 * (4*(maxIterations - iter) % 256) / 256.0 + .4
        return (int(256 * piter), int(256 * piter * piter), int(256 * piter * piter * piter))
    else:
        cbit = int(256 * iter / maxIterations)
        return (cbit, cbit, cbit)
