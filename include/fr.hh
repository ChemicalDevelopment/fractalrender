/* fr.hh - header for the fractalrender program
 *
 *
 * @author: Cade Brown <cade@cade.site>
 */

#ifndef FR_HH
#define FR_HH

#include <vector>
#include <complex>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include <string.h>

namespace fr {

// 8 bit unsigned value, for pixels
using u8 = unsigned char;

// complex type
template<typename T=double>
using complex = std::complex<T>;



// pixel type, using floating point components
struct pixf {

    // pixels structure
    float r, g, b, a;

    pixf(float r_=0.0f, float g_=0.0f, float b_=0.0f, float a_=1.0f) : r(r_), g(g_), b(b_), a(a_) {}


    // mix 2 pixels values
    static pixf mix(const pixf& a, const pixf& b, float t) {
        return pixf(a.r * (1.0f - t) + b.r * t,
                    a.g * (1.0f - t) + b.g * t,
                    a.b * (1.0f - t) + b.b * t,
                    a.a * (1.0f - t) + b.a * t);
    }

};

// pixel type, u8 values
struct pix {

    // Pixel structure
    u8 r, g, b, a;

    pix(u8 r_=0, u8 g_=0, u8 b_=0, u8 a_=255) : r(r_), g(g_), b(b_), a(a_) {}
    pix(const pixf& f) : r(255 * f.r), g(255 * f.g), b(255 * f.b), a(255 * f.a) {}

    // From floating point coordinates in [0, 1]
    //static pix fromf(float r_, float g_, float b_, float a_=1.0f) {
    //    return pix(r_ * 255, g_ * 255, b_ * 255, a_ * 255);
    //}

    // from hex-string (#RRGGBB)
    static pix fromhex(const std::string& s) {
        if (s.size() != 6) {
            throw std::runtime_error("Hex color code should be of length 7 (got '" + s + "')");
        }

        int r, g, b;
        if (3 != sscanf(s.c_str(), "#%02x%02x%02x", &r, &g, &b)) {
            throw std::runtime_error("Hex color code is incorrect format (got '" + s + "'), expected '#RRGGBB'");
        }

        return pix(r, g, b);
    }
    
};

// image structure
struct Image {

    // width/height of image
    int w, h;

    // 2D data array, packed as a single vector, row-wise
    std::vector<pix> data;

    Image(int w_, int h_) : w(w_), h(h_) {
        data.resize(w * h);
        for (int i = 0; i < w * h; ++i) data[i] = pix();
    }

    // Returns the index of the (x, y)th pixel
    int idx(int x, int y) const {
        return y * w + x;
    }

    // Returns a reference of the pixel (x, y)
    pix& operator()(int x, int y) {
        return data[idx(x, y)];
    }

    // write an image to a file
    void write(const std::string& src);

};

// animation structure that can be parsed from a file
struct Anim {

    // a snapshot of the animation
    struct Snap {

        // values defined at this snapshot (key=val), raw string values
        std::map<std::string, std::string> vals;

        // create a new snapshot
        Snap(const std::map<std::string, std::string>& vals_) : vals(vals_) {}

        // get value as integer
        int geti(const std::string& key) const {
            return std::stoi(vals.at(key));
        }

        // get value as double
        double getd(const std::string& key) const {
            return std::stod(vals.at(key));
        }

    };

    // animation snapshots, in order of frames
    std::vector<Snap> snaps;

    // width/height of the animation frames
    int w, h;

    // frames per second that we are rendering
    double fps;

    // the duration, in seconds, of the entire animation
    double dur;

    // number of total frames in the animation
    int nframes;


    // get the time per frame, in seconds
    double get_tpf() const {
        return 1.0 / fps;
    }

    // create from source string, typically from a '.fr' file
    Anim(const std::string& src);

};


// represents an engine structure, which can be used to render a fractal
struct Engine {

    // render a single image
    virtual void render(Image& img, Anim::Snap& snap) = 0;

};

// mandelbrot fractal (z:=z**2+c)
struct MandelbrotEngine : public Engine {

    // implementation
    void render(Image& img, Anim::Snap& snap);

};

/*
/// Utilioty Functions

// Parse complex number from string
complex<double> parsecomplex(const std::string& s);

// Parses a colorscheme (from ',' delimited hex codes) and return it
std::vector<pix> parsescheme(const std::string& s);

// Read entire file contents
std::string readfile(const std::string& fname);

// Writes an image out to a file name. Throws a runtime exception if there was no valid format writer
void writeimg(const std::string& fname, const img& img);

// Replace occurances of 'from' to 'to'. Sad that C++ doesn't have this by default. Just another reason
//   that if I were a hiring manager, anyone who brags that they worked on the C++ spec would get exactly
//   0.0f calls back.
std::string replace(const std::string& str, const std::string& from, const std::string& to);

// fr::Anim - Animation structure, storing keyframes
struct Anim {

    // fr::Anim::KeyFrame - Stores a single position
    struct KeyFrame {

        complex<double> pos;

        double zoom;

        KeyFrame(complex<double> pos_=complex<double>(), double zoom_=0.0) : pos(pos_), zoom(zoom_) {

        }
    };


    // Map of keyframes, keyed on time in seconds
    std::map<double, KeyFrame> frames;

    Anim(const std::string& src) {
        std::string line, tok;
        std::vector<std::string> toks;

        std::istringstream iss(src);
        while (std::getline(iss, line)) {
            // Parse line
            if (line.size() == 0 || line[0] == '#') continue;

            std::istringstream isst(line);
            toks.resize(0);
            while (isst >> tok) {
                toks.push_back(tok);
            }

            if (toks.size() != 3) {
                throw std::runtime_error("Expected exactly 3 tokens per line of animation");
            }

            // Now, parse the tokens
            double t = std::atof(toks[0].c_str());
            complex<double> pos = parsecomplex(toks[1]);
            double zoom = std::atof(toks[2].c_str());
            frames[t] = KeyFrame(pos, zoom);
        }
    }

    // Samples a keyframe at a given time
    KeyFrame sample(double t) const {
        // Get lo and hi, to blend between
        auto lo = frames.upper_bound(t);
        auto hi = frames.upper_bound(t);

        // If only one exists, return the other one
        if (lo == frames.end()) return hi->second;
        lo--;
        if (lo == frames.end()) return hi->second;
        if (hi == frames.end()) return lo->second;


        // Blending proportion
        double prop = (t - lo->first) / (hi->first - lo->first);

        return KeyFrame(
            (1 - t) * lo->second.pos + t * hi->second.pos,
            exp((1 - t) * log(lo->second.zoom) + t * log(hi->second.zoom))
        );
    }

};


// fr::Engine - Base class of a rendering engine capable of producing a fractal image
struct Engine {

    // Generic dictionary
    std::unordered_map<std::string, std::string> defs;

    // Convert pixel coordinates to position, via center/zoom
    template<typename T=double>
    static complex<T> pix2pos(int x, int y, int w, int h, complex<T> center, T zoom) {
        return complex<T>(
        center.real() + (1.0 / zoom) * (2 * x - w) / w,
        center.imag() + (1.0 / zoom) * (h - 2 * y) / w
        );
    }

    // Overide this to actually implement rendering
    virtual void render(IMG& img, complex<double> center, double zoom) = 0;

};

// fr::EngineMandelbrot - Example implementation of the mandelbrot fractal (z**2+c)
//
// Implemented in EngineMandelbrot.cc
struct EngineMandelbrot : public Engine {

    void render(IMG& img, complex<double> center, double zoom);

};




// Utility functions

// Returns |x|^2
template<typename T=double>
static T normsqr(complex<T> x) {
    return x.real() * x.real() + x.imag() * x.imag();
}

// Sample a colorscheme, with blending
static PIX samplescheme(const std::vector<PIX>& scheme, double idx) {

    // Get mix factor
    double mixfac = idx - floor(idx);

    // Get integer indices
    int i0 = ((int)idx) % scheme.size();
    int i1 = (i0 + 1) % scheme.size();

    // Sample colors
    PIX p0 = scheme[i0], p1 = scheme[i1];

    // Now, mix the colors
    return PIX(
        (1.0 - mixfac) * p0.r + mixfac * p1.r,
        (1.0 - mixfac) * p0.g + mixfac * p1.g,
        (1.0 - mixfac) * p0.b + mixfac * p1.b
    );
}

*/

}

#endif // FR_HH
