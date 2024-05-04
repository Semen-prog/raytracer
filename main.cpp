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
        10,               // max depth
        point3(13, 2, 3), // lookfrom
        point3(0, 0, 0),  // lookat
        vec3(0, 1, 0),    // dir up
        rad(0)          // defocus angle
    );

    auto ground_material = new Lamberitan(color(0.5, 0.5, 0.5));
    vp.world.add(new Sphere(point3(0,-1000,0), 1000), ground_material);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_01();
            point3 center(a + 0.9 * random_01(), 0.2, b + 0.9 * random_01());

            if ((center - point3(4, 0.2, 0)).len() > 0.9) {
                Style *sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_color() & random_color();
                    sphere_material = new Lamberitan(albedo);
                    vp.world.add(new Sphere(center, 0.2), sphere_material);
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_color() * 0.5 + color(0.5, 0.5, 0.5);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = new Metal(albedo, fuzz);
                    vp.world.add(new Sphere(center, 0.2), sphere_material);
                } else {
                    // glass
                    sphere_material = new Dielectric(1.5);
                    vp.world.add(new Sphere(center, 0.2), sphere_material);
                }
            }
        }
    }

    auto material1 = new Dielectric(1.5);
    vp.world.add(new Sphere(point3(0, 1, 0), 1.0), material1);

    auto material2 = new Lamberitan(color(0.4, 0.2, 0.1));
    vp.world.add(new Sphere(point3(-4, 1, 0), 1.0), material2);

    auto material3 = new Metal(color(0.7, 0.6, 0.5), 0.0);
    vp.world.add(new Sphere(point3(4, 1, 0), 1.0), material3);

    vp.render(std::cout);
}