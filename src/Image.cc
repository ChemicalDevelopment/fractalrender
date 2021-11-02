/* Image.cc - implementation of 'fr::Image'
 *
 * @author: Cade Brown <cade@cade.site>
 */

#include <fr.hh>

// use the STB library for image writing
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


namespace fr {

// Writes an image out to a file name. Throws a runtime exception if there was no valid format writer
void Image::write(const std::string& src) {
    // Find extension
    const char* ext = strrchr(src.c_str(), '.');
    if (!ext) {
        throw std::runtime_error("no valid extension for filename: " + src);
    }

    std::cout << "writing " << src << "..." << std::endl;
    if (strcmp(ext, ".png") == 0) {
        int rc = stbi_write_png(src.c_str(), w, h, 4, &data[0], sizeof(pix) * w);
    } else {
        throw std::runtime_error("no valid extension for filename (expected '.png'): " + src);
    }
}

}
