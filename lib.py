#Returns the iterations of (x+iy). Returns 0 through maxIterations inclusive
def rawIterations(x, y, maxIterations):
    re = x
    im = y
    iter = 0
    while re * re + im * im < 4 and iter < maxIterations:
        tmp = 2 * re * im
        re = re * re - im * im + x
        im = tmp + y
        iter += 1
    return iter

#Colorizes based on pattern
def colorize(pattern, iter, maxIterations):
    pattern = pattern.upper()
    if pattern == "MOCHA":
        piter = (iter + 0.0) / maxIterations
        return (int(256 * piter), int(256 * piter * piter), int(256 * piter * piter * piter))