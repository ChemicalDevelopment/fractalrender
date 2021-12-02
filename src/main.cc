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

    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <file.fr> <outdir>" << std::endl;
        std::cerr << "fractalrender (fr) version 0.0.1" << std::endl;
        std::cerr << "author: Cade Brown <me@cade.site>" << std::endl;
        return 1;
    }


    // zoom per frame, as a rate
    double zpf = 1.618;

    // width and height of each frame
    int w = 1920, h = 1080;

    // 'z' coordinate
    double zre = 0.2821, zim = 0.01;



    // create new engine to execute
    fr::Engine* eng = new fr::MandelbrotEngine();

    // output directory
    std::string outdir = argv[2];

    // time per frame
    double tpf = anim.get_tpf();

    // make directory for output
    if (mkdir(outdir.c_str(), 0755) != 0 && errno != EEXIST) {
        std::cerr << "error: failed to create output directory" << std::endl;
        return 1;
    }

    // time
    double t;
    // frame index
    int i;
    for (i = 0, t = 0.0; t <= anim.dur; t += tpf, i++) {
        // get current snapshot for animation
        anim.snaps.push_back(fr::Anim::Snap({}));
        fr::Anim::Snap& snap = anim.snaps[i];
        //snap.vals.insert({ "zoom", std::to_string(pow(1.8, t) * 0.5) });
        snap.vals.insert({ "zoom", std::to_string(pow(30, t) * 100) });

        char tmp[PATH_MAX];
        snprintf(tmp, sizeof(tmp), "%s/frame_%05d.png", outdir.c_str(), i);

        // output fiile
        std::string outfile = tmp;
        std::cout << "rendering frame " << i << " (" << t << "s/" << anim.dur << "s) ..." << std::endl;

        // create new image
        fr::Image img = fr::Image(anim.w, anim.h);

        // render image
        eng->render(img, snap);

        // write to disk
        img.write(outfile);
    }

    std::cout << "  done! to string together, run: " << std::endl;
    std::cout << "  $ ffmpeg -r " << anim.fps << " -f image2 -s " << anim.w << "x" << anim.h << " -i " << outdir << "/frame_%05d.png -vcodec libx264 -crf 25 -pix_fmt yuv420p out.mp4" << std::endl;
}

