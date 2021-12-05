/* main.cc - CLI for fractalrender
 *
 * @author: Cade Brown <cade@cade.site>
 */

#include <fr.hh>

#include <sys/stat.h>
#include <errno.h>

#ifndef PATH_MAX
  #define PATH_MAX 4096
#endif

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <outdir>" << std::endl;
        std::cerr << "fractalrender (fr) version 0.0.1" << std::endl;
        std::cerr << "author: Cade Brown <me@cade.site>" << std::endl;
        return 1;
    }


    // output directory
    std::string outdir = argv[1];


    // width and height of each frame
    int w = 1920, h = 1080;

    // center coordinate
    double Cx = 0.2821, Cy = 0.01;

    // max iteration count
    int maxiter = 1000;

    // initial zoom (at t=0)
    double z0 = 1.0;

    // zoom per second
    double zps = 15.0;

    // frames per second being rendered
    double fps = 5.0;

    // duratio nin seconds
    double dur = 10.0;


    // create new engine to execute
    fr::Engine* eng = new fr::MandelbrotEngine();


    // make directory for output
    if (mkdir(outdir.c_str(), 0755) != 0 && errno != EEXIST) {
        std::cerr << "error: failed to create output directory" << std::endl;
        return 1;
    }

    // create an image to share
    fr::Image img = fr::Image(w, h);

    double tpf = 1.0 / fps;

    // frame index
    int i;
    double t;
    for (i = 0, t = 0.0; t < dur; t += tpf, i++) {

        // calculate output path
        char tmp[PATH_MAX];
        snprintf(tmp, sizeof(tmp), "%s/frame_%05d.png", outdir.c_str(), i);

        // output fiile
        std::string outfile = tmp;
        std::cout << "rendering frame " << i << " (" << t << "s/" << dur << "s) ..." << std::endl;

        // calculate zoom
        double zoom = z0 * pow(zps, t);

        // render image
        eng->render(img, Cx, Cy, zoom, maxiter);

        // write to disk
        img.write(outfile);
    }

    std::cout << "  done! to string together, run: " << std::endl;
    //std::cout << "  $ ffmpeg -r " << fps << " -f image2 -s " << anim.w << "x" << anim.h << " -i " << outdir << "/frame_%05d.png -vcodec libx264 -crf 25 -pix_fmt yuv420p out.mp4" << std::endl;
    std::cout << "  $ ffmpeg -y -i " << outdir << "/frame_%05d.png -framerate " << fps << " out.mp4" << std::endl;
}

