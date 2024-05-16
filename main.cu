#define ENABLE_CUDA

#include <cuda_runtime.h>
#include "libs/includes.hpp"

__device__ Viewport vp;
__device__ int pw, ph, mt;

void load_image(int &w, int &h, color *&darr, std::string path) {
    color *arr;
    parse_image(w, h, arr, path);
    cudaMalloc(&darr, w * h * sizeof(color));
    cudaMemcpy(darr, arr, w * h * sizeof(color), cudaMemcpyHostToDevice);
}

__global__ void init(int imw, int imh, color *imarr) {
    vp = Viewport(
        pw,                     // pixel width
        ph,                     // pixel height
        rad(20),                // field of view (rad)
        500,                    // samples
        0.5,                    // x-offset for sampling
        0.5,                    // y-offset for sampling
        10,                     // max depth
        point3(8, 2, 3),        // lookfrom
        point3(0.5, 0.5, 0.5),  // lookat
        vec3(0, 1, 0),          // dir up(int)(darr[0].x + 2)
        rad(0),                 // defocus angle
        1000                    // max fig cnt
    );

    auto ground_material = new Lamberitan(new Solid(color(1, 1, 1)));
    vp.world.add(new Parallelepiped(point3(-1, -10, -1), point3(9, -10, -1), point3(9, -10, 4), point3(-1, -10, 4), point3(-1, 3, -1), point3(9, 3, -1), point3(9, 3, 4), point3(-1, 3, 4)), ground_material);

    auto light_material = new Lamp(color(1, 1, 1));
    vp.world.add(new Parallelepiped(point3(-1, 0.5, 1.5), point3(-0.5, 0.5, 1.5), point3(-0.5, 0.5, 2.5), point3(-1, 0.5, 2.5), point3(-1, 1.5, 1.5), point3(-0.5, 1.5, 1.5), point3(-0.5, 1.5, 2.5), point3(-1, 1.5, 2.5)), light_material);

    auto par_material = new Lamberitan(new Solid(color(5.0 / 255, 107.0 / 255, 0)));
    for (double i = 0; i < 5.3; i += 0.5) {
        for (double j = 0; j < 5.3; j += 0.5) {
            double h = random_double(-0.5, 0.5);
            vp.world.add(new Parallelepiped(point3(i, -10, j), point3(i + 1, -10, j), point3(i + 1, -10, j + 1), point3(i, -10, j + 1), point3(i, h, j), point3(i + 1, h, j), point3(i + 1, h, j + 1), point3(i, h, j + 1)), par_material);
        }
    }

    auto earth_material = new Lamberitan(new Imtext(imw, imh, imarr));
    vp.world.add(new Sphere(point3(0.5, 1, 0.5), 0.5), earth_material);
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

    int imw, imh; color *imarr;
    load_image(imw, imh, imarr, "earthmap.ppm");
    
    init<<<1, 1>>>(imw, imh, imarr);
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