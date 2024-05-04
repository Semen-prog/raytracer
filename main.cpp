#include "libs/includes.h"

Viewport vp;

int main() {
    int pixel_width = 1366, pixel_height = 768;
    vp = Viewport(
        pixel_width,      // pixel width
        pixel_height,     // pixel height
        rad(20),          // field of view (rad)
        1.0,              // focal length
        10,               // samples per pixel
        0.5,              // x-offset for sampling
        0.5,              // y-offset for sampling
        50,               // max depth
        point3(0, 0, 0),  // lookfrom
        point3(0, 0, 10),  // lookat
        vec3(0, 1, 0),    // dir up
        rad(0)          // defocus angle
    );

    auto ground_material = new Lamberitan(color(0.5, 0.5, 0.5));
    vp.world.add(new Sphere(point3(0,-1000,40), 1000), ground_material);

    auto material = new Dielectric(1.5);
    auto material_sphere = new Lamberitan(color(0.5, 0.5, 0.5));
    //vp.world.add(new Sphere(point3(1, 1, 11), 0.1), material);
    //vp.world.add(new Sphere(point3(1, 1, 12), 0.1), material);
    //vp.world.add(new Sphere(point3(1, 0, 12), 0.1), material);
    //vp.world.add(new Sphere(point3(0, 1, 12), 0.1), material);
    vp.world.add(new Pyramid(point3(0.5, -0.2, 9), point3(-0.5, -0.2, 9), point3(0.1, 0.3, 8), point3(0, 0.8, 8.5)), material);
    vp.world.add(new Sphere(point3(-0.9, 0.5, 8.5), 0.5), material_sphere);

    vp.render(std::cout);
}