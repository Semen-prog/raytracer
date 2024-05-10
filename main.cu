#define ENABLE_CUDA

#include <cuda_runtime.h>
#include "libs/includes.hpp"

__device__ Viewport vp;
__device__ int pw, ph, mt;

void load_image(int &w, int &h, color *&darr, std::string path) {
    color *arr;
    parse(w, h, arr, path);
    cudaMalloc(&darr, w * h * sizeof(color));
    cudaMemcpy(darr, arr, w * h * sizeof(color), cudaMemcpyHostToDevice);
}

__global__ void init(int imw, int imh, color *darr) {
    vp = Viewport(
        pw,               // pixel width
        ph,               // pixel height
        rad(20),          // field of view (rad)
        50,               // samples per pixel
        0.5,              // x-offset for sampling
        0.5,              // y-offset for sampling
        50,               // max depth
        point3(13, 2, 3), // lookfrom
        point3(0, 0, 0),  // lookat
        vec3(0, 1, 0),    // dir up(int)(darr[0].x + 2)
        rad(0),           // defocus angle
        1000               // max fig cnt
    );

    auto ground_material = new Lamberitan(new Solid(color(0.5, 0.5, 0.5)));
    vp.world.add(new Sphere(point3(0,-1000,0), 1000), ground_material);

    for (int a = -11; a < 11; a += 1) {
        for (int b = -11; b < 11; b += 1) {
            auto choose_mat = random_01();
            point3 center(a + 0.9 * random_01(), 0.2, b + 0.9 * random_01());

            if ((center - point3(4, 0.2, 0)).len() > 0.9) {
                Style *sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_color(0, 1) * random_color(0, 1);
                    sphere_material = new Lamberitan(new Solid(albedo));
                    vp.world.add(new Sphere(center, 0.2), sphere_material);
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_color(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = new Metal(fuzz, new Solid(albedo));
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
    auto material2 = new Lamberitan(new Imtext(imw, imh, darr));
    auto material3 = new Metal(0.0, new Checker(2.0, color(0, 1, 0), color(0.7, 0.6, 0.5)));

    vp.world.add(new Sphere(point3(0, 1, 0), 1.0), material1);
    vp.world.add(new Sphere(point3(-4, 1, 0), 1.0), material3);
    vp.world.add(new Sphere(point3(4, 1, 0), 1.0), material2);
}

__global__ void render(color *arr) {
    int i = blockIdx.x * mt + threadIdx.x;
    if (i >= pw * ph) return;
    arr[i] = prepare_color(vp.get_pixel_color(i % pw, i / pw));
}

int main() {
    int pixel_width = 1366, pixel_height = 768, max_thread = 512;
    cudaMemcpyToSymbol(pw, &pixel_width, sizeof(int));
    cudaMemcpyToSymbol(ph, &pixel_height, sizeof(int));
    cudaMemcpyToSymbol(mt, &max_thread, sizeof(int));

    color *darr; int imw, imh;
    load_image(imw, imh, darr, "earthmap.ppm");
    
    init<<<1, 1>>>(imw, imh, darr);
    cudaDeviceSynchronize();

    int size = pixel_height * pixel_width;

    color *phost = (color*)malloc(size * sizeof(color)), *pdevice;
    cudaMalloc(&pdevice, size * sizeof(color));
    
    render<<<(size + max_thread - 1) / max_thread, max_thread>>>(pdevice);
    cudaMemcpy(phost, pdevice, size * sizeof(color), cudaMemcpyDeviceToHost);

    std::cout << "P3\n" << pixel_width << ' ' << pixel_height << "\n255\n";
    for (int i = 0; i < size; ++i) {
        auto pixel_formed = phost[i];
        std::cout << std::round(pixel_formed.x) << ' ' << std::round(pixel_formed.y) << ' ' << std::round(pixel_formed.z) << '\n';
    }
    std::cout << std::endl;
}