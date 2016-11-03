#Currently testing
try:
    from mpmath import *
except:
    from math import *
    print "mpmath not found, going along as normal"

import random

class Context():
    def __init__(self, dim, center, zoom, zps,iter, pattern, func, threads, sec, fps):
        self.dim = dim
        self.center = center
        self.zoom = zoom
        self.zps = zps
        self.iter = int(iter)
        self.pattern = pattern
        self.func = func
        self.threads = threads
        self.fps = fps
        self.sec = sec

        self.zoom_eq = "%s*(%s**t)" % (zoom, zps)
        self.frames = int(fps * sec)
        self.get_val(0)

    def get_val(self, frame):
        self.time = self.sec * ((frame + 0.0) / (self.frames))
        t = self.time
        self.zoom = eval(self.zoom_eq)
        z = self.zoom
        self.start_y = self.center[1] + 1.0 / (z) * ((self.dim[1] + 0.0) / self.dim[0])
        self.start_x = self.center[0] - 1.0 / z
        self.change = 2.0 / (z * self.dim[0])

def get_color(context, x, y):
    return colorize(context, rawIterations(context, x, y))

#Returns the iterations of (x+iy). Returns 0 through maxIterations inclusive
def rawIterations_mand(context, x, y):
    re = x
    im = y
    iter = 0
    while re * re + im * im < 4 and iter < context.iter:
        tmp = 2 * re * im
        re = re * re - im * im + x
        im = tmp + y
        iter += 1
    return iter

#Returns the iterations of (x+iy). Returns 0 through maxIterations inclusive
def rawIterations(context, x, y):
    if not context.func or context.func == "z**2+c":
        return rawIterations_mand(context, x, y)
    z = 0 + 0j
    c = x + y*1j
    iter = 0
    t = context.time
    while z.imag * z.imag + z.real * z.real <= 4 and iter < context.iter:
        try:
            z = eval(context.func)
        except:
            z = c
        #tmp = 2 * re * im
        #re = re * re - im * im + x
        #im = tmp + y
        iter += 1
    return iter

#Colorizes based on pattern
def colorize(context, iter):
    context.pattern = context.pattern.upper()
    if context.pattern == "MOCHA":
        piter = .6 * (4*(context.iter - iter) % 256) / 256.0 + .4
        return (int(256 * piter), int(256 * piter * piter), int(256 * piter * piter * piter))
    else:
        cbit = int(256 * iter / context.iter)
        return (cbit, cbit, cbit)
