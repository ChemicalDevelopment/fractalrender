/* Mandelbrot.cc - implementation of fr::MandelbrotEngine
 *
 * z**2 + c
 *
 * @author: Cade Brown <cade@cade.site>
 */

#include <fr.hh>

namespace fr {

void MandelbrotEngine::render(Image& img, Anim::Snap& snap) {
    int w = img.w, h = img.h;

    // get maximum iteration count
    int maxiter = 100; //snap.geti("maxiter");

    // center values
    double czre = 0.2821, czim = 0.01;

    // current zoom level
    //double zoom = 0.5;
    double zoom = snap.getd("zoom");

    // precompute, so we don't have to do it in the loop
    double invzoom = 1.0 / zoom;

    // error squared, for escape radius
    // minimum should be 4=2**2, but larger values reduce seams when gradients are produced
    double err2 = 256.0;

    // epsilon to check convergence for periods/cycles
    double eps_old = 1e-6;

    std::vector<pixf> scheme = {
        pixf(1.0f, 0, 0),
        pixf(0, 1.0f, 0),
        pixf(0, 0, 1.0f),
    };

    // Binary splitting (binary decomposition based on last coordinates)
    //bool do_bsr = std::stoi(defs["bsr"]), do_bsi = std::stoi(defs["bsi"]);

    // Now, compute for each pixel
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // get initial value '
            double cre = czre + invzoom * (2.0 * x - w) / h;
            double cim = czim + invzoom * (h - 2.0 * y) / h;

            // z := c
            double zre = cre;
            double zim = cim;

            // period checking
            int per = 0;
            double old_zre = 0.0, old_zim = 0.0;

            int i;
            for (i = 0; i < maxiter && zre*zre + zim*zim < err2; ++i) {
                // on each iteration, compute:
                // z := z**2 + c
                // which is:
                // z.re = z.re**2 - z.im**2 + c.re
                // z.im = 2*z.re*z.im + c.im
                double tmp = 2 * zre * zim + cim;
                zre = zre * zre - zim * zim + cre;
                zim = tmp;

                // check similar value
                if (fabs(zre - old_zre) < eps_old && fabs(zim - old_zim) < eps_old) {
                    // skip ahead, since we are sure we are in a cycle
                    i = maxiter;
                    break;
                }

                // now, update period
                per++;
                if (per > 20) {
                    per = 0;
                    old_zre = zre;
                    old_zim = zim;
                }
            }

            if (i == maxiter) {
                // inside the set, so set to black
                img(x, y) = pix();
            } else {
                // compute color based on iteration count
                //int c = (i * 255) / maxiter;
                //img.set(x, y, Color(c, c, c));

                float fri = 2 + i - logf(logf((float)(zre*zre + zim*zim))) / logf(2.0);

                // Binary splitting on final coordinate (real/imag)
                //if (do_bsr && z.real() > 0) fri += 1.0;
                //if (do_bsi && z.imag() > 0) fri += 2.0;

                // Apply color & scale
                //fri = 0.25f * fri + 0.0f;

                // break up into components
                float frif = fmodf(fri, 1.0);
                float frii = fri - frif;

                // sample scheme
                int x0 = ((int)frii) % scheme.size();
                int x1 = (x0 + 1) % scheme.size();

                // now, mix
                img(x, y) = pixf::mix(scheme[x0], scheme[x1], frif);
            }

            /*
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
            */
        }
    }
}
}
