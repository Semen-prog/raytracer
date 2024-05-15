#include "libs/includes.hpp"

int pw, ph;
Viewport vp;

void load_image(int &w, int &h, color *&darr, std::string path) {
    parse_image(w, h, darr, path);
}

void init() {
    vp = Viewport(
        pw,               // pixel width
        ph,               // pixel height
        rad(20),          // field of view (rad)
        50,               // samples per pixel
        0.5,              // x-offset for sampling
        0.5,              // y-offset for sampling
        50,               // max depth
        point3(4, 2, 6), // lookfrom
        point3(0.5, 0.5, 0.5),  // lookat
        vec3(0, 1, 0),    // dir up(int)(darr[0].x + 2)
        rad(0),           // defocus angle
        1000               // max fig cnt
    );

    auto ground_material = new Lamberitan(new Solid(color(0.5, 0.5, 0.5)));
    vp.world.add(new Sphere(point3(0,-1000,0), 1000), ground_material);

    auto rnd_material = new Metal(0, new Solid(color(0.3, 1, 0.3)));
    auto par_material = new Metal(0, new Solid(color(1, 0, 0)));
    vp.world.add(new Sphere(point3(2.5, 0.5, 0.5), 0.5), rnd_material);
    vp.world.add(new Parallelepiped(point3(0, 0, 0), point3(1, 0, 0), point3(1, 0, 1), point3(0, 0, 1), point3(0, 1, 0), point3(1, 1, 0), point3(1, 1, 1), point3(0, 1, 1)), par_material);
}

void render(color *arr, int i) {
    arr[i] = prepare_color(vp.get_pixel_color(i % pw, i / pw));
}

int main() {
    int pixel_width = 1366, pixel_height = 768;
    pw = pixel_width;
    ph = pixel_height;

    init();
    color *phost = (color*)malloc(pixel_height * pixel_width * sizeof(color));
    for (int i = 0; i < pixel_height * pixel_width; ++i) {
        render(phost, i);
    }
    std::cout << "P3\n" << pixel_width << ' ' << pixel_height << "\n255\n";
    for (int i = 0; i < pixel_height * pixel_width; ++i) {
        auto pixel_formed = phost[i];
        std::cout << std::round(pixel_formed.x) << ' ' << std::round(pixel_formed.y) << ' ' << std::round(pixel_formed.z) << '\n';
    }
    std::cout << std::endl;
}