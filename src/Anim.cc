/* Anim.cc - implementation of 'fr::Anim'
 *
 * z**2 + c
 *
 * @author: Cade Brown <cade@cade.site>
 */

#include <fr.hh>

namespace fr {

Anim::Anim(const std::string& src) {

    // require a current character
    #define REQ(_c) if (src[i++] != _c) { throw std::runtime_error(std::string("expected '") + _c + "' (line " + std::to_string(line) + ")"); }
    
    w = 640;
    h = 480;
    fps = 24.0;

    snaps.push_back(Snap({}));

    
    // TODO: parse numbers

    return;

    size_t i = 0;
    int line = 1;
    while (i < src.size()) {
        // skip whitespace
        while (i < src.size() && (src[i] == ' ' || src[i] == '\t')) {
            i++;
        }

        std::cout << "GOT: " << src[i] << std::endl;

        if (src[i] == '#') {
            // comment, skip this line
        } else if ('0' <= src[i] && src[i] <= '9') {
            // number, which means a valid keyframe
        } else if (src[i] == 'w') {
            // assume first line (w, h, f)
            REQ('w');
            REQ('=');

            REQ('h');
            REQ('=');

            REQ('f');
            REQ('=');

        } else {
            throw std::runtime_error("invalid character in animation file (line " + std::to_string(line) + ")");
        }

        while (i < src.size() && src[i] != '\n') i++;
        i++;
        line++;
    }

}

}
