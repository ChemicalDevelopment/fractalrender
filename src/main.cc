/* main.cc - Commandline interface to fractalrender
 *
 * @author: Cade Brown <cade@cade.site>
 */

#include <fr.hh>

// For argument parsing
#include <getopt.h>



int main(int argc, char** argv) {


    // Image dimension
    int w = 640, h = 480;
    // Center of image
    double center_r = 0.0, center_i = 0.0;
    // Zoom
    double zoom = 1.0f;

    // Definitions for the engine
    std::unordered_map<std::string, std::string> defs;

    // Add some defaults
    defs["iter"] = "256";
    defs["color"] = "01295F,BDD5EA,F7F7FF,A39171,157A6E";
    defs["bsr"] = "0";
    defs["bsi"] = "0";
    defs["colorscale"] = "0.25";
    defs["coloroffset"] = "0.0";

    // Output filename
    std::string fname = "out.png";

    // Now, parse the arguments given
    char opt;
    while ((opt = getopt(argc, argv, "s:c:z:o:h")) != -1) {
        switch ( opt ) {
            case 's':
                sscanf(optarg, "%dx%d", &w, &h);
                break;
            case 'c':
                char sign;
                sscanf(optarg, "%lf%c%lfi", &center_r, &sign, &center_i);
                if (sign == '-') center_i = -center_i;
                break;
            case 'z':
                sscanf(optarg, "%lf", &zoom);
                break;
            case 'o':
                fname = optarg;
                break;
            case 'h':
                printf("Usage: %s -oOUT [-sWxH] [-cR+Ii] [-zZOOM] [name=val ...]\n", argv[0]);
                printf("    -o OUT            Can be .png, .bmp, or .jpg file\n");
                printf("    -s WxH            Size (in pixels) width by height\n");
                printf("    -c R+Ii           Center of the image in the complex plane\n");
                printf("    -z ZOOM           Zoom of the image in the complex plane (higher zoom is more zoomed in)\n");
                printf("    name=val          A key (string) and value (scalar) pair, which is passed along to the fractal. Common examples are `iter=100` `colscale=2.5`, etc\n");
                
                printf("\n");
                printf("fractalrender (C++ version), written by Cade Brown <cade@cade.site>\n");
                printf("Version %d.%d\n", 1, 0);

                exit(0);
                break;
            case '?':  // unknown option...
                fprintf(stderr, "Error, unknown option -%c\n", (char)optopt);
                exit(1);
                break;
        }
    }

    // The rest of the arguments are added to the definitions
    for (int index = optind; index < argc; index++) {
        const char* eqs = strrchr(argv[index], '=');
        if (!eqs) {
            printf("Error for parameter: '%s'\n", argv[index]);
            return 1;
        }

        // Now, set in the definitions
        defs[std::string((const char*)argv[index], eqs)] = std::string(eqs + 1);
    }

    // Create the engine used to render
    fr::Engine* eng = new fr::EngineMandelbrot();
    eng->defs = defs;

    // Construct an empty image
    fr::IMG img = fr::IMG(w, h);

    // Perform computation
    eng->render(img, fr::complex<double>(center_r, center_i), zoom);

    // Now, output it
    fr::writeimg(fname, img);
}

