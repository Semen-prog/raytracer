#include "LibTrace/scene.hpp"

Color Scene::get_ray_color(int x, int y, int depth, Hitinfo hitinfo) const {
    Color result(1, 1, 1);
    Ray tmp = viewport->get_ray(x, y);
    for (int i = 0; i < depth; ++i) {
        if (almost_black(result)) break;
        figure_record fr;
        if (!list->hit(tmp, fr, hitinfo.h)) return result;
        if (hitinfo.r == PreviewRender) {
            result = result * Color(0.5, 0.5, 0.5);
            return result;
        }
        result = result * fr.a;
        if (!fr.r) return result;
        tmp = fr.s;
    }
    return Color(0, 0, 0);
}
QColor Scene::get_pixel_color(int x, int y, Hitinfo hitinfo) const {
    Color fin(0, 0, 0);
    for (int _ = 0; _ < samples_per_pixel; ++_) {
        fin = fin + get_ray_color(x, y, max_depth, hitinfo);
    }
    return correct(fin / samples_per_pixel);
}

QImage Scene::render(Hitinfo hitinfo) {
    if (hitinfo.h == HardHit) list->build();
    int pw = viewport->pixel_width, ph = viewport->pixel_height;
    QImage img(pw, ph, QImage::Format_ARGB32);
    for (int y = 0; y < ph; ++y) {
        for (int x = 0; x < pw; ++x) {
            img.setPixelColor(x, y, get_pixel_color(x, y, hitinfo));
        }
    }
    return img;
}