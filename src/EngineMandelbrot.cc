/* EngineMandelbrot.cc - Implementation of the mandelbrot engine
 *
 * z**2 + c
 *
 * @author: Cade Brown <cade@cade.site>
 */

#include <fr.hh>

namespace fr {

void EngineMandelbrot::render(IMG& img, complex<double> center, double zoom) {

    // Capture vars
    int w = img.w, h = img.h;

    // Get iteration count
    int iter = std::stoi(defs["iter"]);

    // Get color scheme
    std::vector<PIX> scheme = parsescheme(defs["color"]);

    // Error squared, for escape radius
    // Minimum should be 4=2**2, but larger values reduce seams when gradients are produced
    double err2 = 256.0;

    // Binary splitting (binary decomposition based on last coordinates)
    bool do_bsr = std::stoi(defs["bsr"]), do_bsi = std::stoi(defs["bsi"]);

    // Now, compute for each pixel
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            complex<double> c = pix2pos(x, y, w, h, center, zoom);
            complex<double> z = c;

            // Iterate the equation until it escapes or we run out of iterations
            int i;
            for (i = 0; i < iter && normsqr(z) < err2; ++i) {
                z = z * z + c;
            }

            // Whether the point is in the set
            bool in_set = i == iter;

            if (in_set) {
                // Constant coloring
                //img(x, y) = scheme[0];
                img(x, y) = PIX();
            } else {
                // Use sampling of intermediate indices

                // Using this formula is a good way of getting smooth values, as it interpolates between norms
                //   in a linear way (repeated squaring should square the magnitude, assuming no catastrophic cancellation with the `+ c` term)
                float fri = 2 + i - logf(logf(normsqr(z))) / logf(2.0);

                // Binary splitting on final coordinate (real/imag)
                if (do_bsr && z.real() > 0) fri += 1.0;
                if (do_bsi && z.imag() > 0) fri += 2.0;

                // Apply color & scale
                fri = 0.25f * fri + 0.0f;

                img(x, y) = samplescheme(scheme, fri);
            }
        }
    }
}

}
