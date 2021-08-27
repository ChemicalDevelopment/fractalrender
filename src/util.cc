/* util.cc - Misc. utilities
 *
 * @author: Cade Brown <cade@cade.site>
 */

#include <fr.hh>

// Use the STB library
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


namespace fr {

std::string readfile(const std::string& fname) {
    std::ifstream ifs(fname);
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

std::string replace(const std::string& str, const std::string& from, const std::string& to) {
    std::string text = str;
    for (auto at = text.find(from, 0); at != std::string::npos; at = text.find(from, at + to.length())) {
        text.replace(at, from.length(), to);
    }
    return text;
}

// Writes an image out to a file name. Throws a runtime exception if there was no valid format writer
void writeimg(const std::string& fname, const IMG& img) {
    const char* fns = fname.c_str();
    // Find extension
    const char* ext = strrchr(fns, '.');
    if (!ext) {
        throw std::runtime_error("No valid extension for filename: " + fname);
    }

    std::cout << "writing " << fname << std::endl;
    if (strcmp(ext, ".png") == 0) {
        int rc = stbi_write_png(fns, img.w, img.h, 4, &img.data[0], sizeof(PIX) * img.w);
    } else {
        throw std::runtime_error("No valid extension for filename (expected '.png'): " + fname);
    }
}

complex<double> parsecomplex(const std::string& s) {
    char sign;
    double re, im;
    sscanf(s.c_str(), "%lf%c%lfi", &re, &sign, &im);
    if (sign == '-') im = -im;
    return complex<double>(
        re,
        im
    );
}

std::vector<PIX> parsescheme(const std::string& s) {
    std::istringstream ss(s); // insert the string into tokenizer
    std::string token;
    std::vector<PIX> res;

    while (getline(ss, token, ',')) {
        // Push back color parsed from hex
        res.push_back(PIX::fromhex(token));
    }

    // Add white if nothing else
    if (res.size() == 0) {
        res.push_back(PIX(255, 255, 255));
    }

    return res;
}

}
