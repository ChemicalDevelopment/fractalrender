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

    // from hex-string (#RRGGBB)
    static pixf fromhex(const std::string& s) {
        if (s.size() != 7) {
            throw std::runtime_error("Hex color code should be of length 7 (got '" + s + "')");
        }

        int r, g, b;
        if (3 != sscanf(s.c_str(), "#%02x%02x%02x", &r, &g, &b)) {
            throw std::runtime_error("Hex color code is incorrect format (got '" + s + "'), expected '#RRGGBB'");
        }

        return pixf(r / 255.0f, g / 255.0f, b / 255.0f);
    }

    // create scheme from CSV colors
    // i.e.:
    // "f9c80e,f86624,ea3546,662e9b,43bccd"
    static std::vector<pixf> scheme(const std::string& colors_) {
        std::string colors = colors_;
        std::vector<std::string> split = {};
        size_t pos = 0;
        std::string delim = ",";
        while ((pos = colors.find(delim)) != std::string::npos) {
            std::string token = colors.substr(0, pos);
            if (token.length() > 0) split.push_back(token);
            colors.erase(0, pos + delim.length());
        }

        if (colors.length() > 0) split.push_back(colors);

        std::vector<pixf> res;
        for (auto& s : split) {
            pixf p = pixf::fromhex("#" + s);
            res.push_back(p);
        }
        return res;
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
        if (s.size() != 7) {
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


// represents an engine structure, which can be used to render a fractal
struct Engine {

    // render a single image, given center position, zoom and maximum iteration count
    virtual void render(Image& img, double Cx, double Cy, double zoom, int maxiter) = 0;

};

// mandelbrot fractal (z:=z**2+c)
struct MandelbrotEngine : public Engine {

    // implementation
    void render(Image& img, double Cx, double Cy, double zoom, int maxiter);
    
};
// burning ship fractal
struct BurningShipEngine : public Engine {

    // implementation
    void render(Image& img, double Cx, double Cy, double zoom, int maxiter);
    
};

/*

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

*/


}

#endif // FR_HH
