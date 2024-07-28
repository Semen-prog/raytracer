#pragma once

#include <QImage>
#include "LibTrace/viewport.hpp"
#include "LibTrace/list.hpp"
#include "LibTrace/color.hpp"

enum Rendertype {
    PreviewRender,
    FullRender
};

struct Hitinfo {
    Hittype h;
    Rendertype r;
    Hitinfo(Hittype _h, Rendertype _r): h(_h), r(_r) {}
};

class Scene {
   public:
    QSharedPointer<FigureList> list;
    QSharedPointer<Viewport> viewport;
    int samples_per_pixel;
    int max_depth;
    Scene() = default;
    Scene(QSharedPointer<FigureList> fl, QSharedPointer<Viewport> vp, int spp, int md) : list(fl), viewport(vp), samples_per_pixel(spp), max_depth(md) {}
    Color get_ray_color(int x, int y, int depth, Hitinfo hitinfo) const;
    QColor get_pixel_color(int x, int y, Hitinfo hitinfo) const;
    QImage render(Hitinfo hitinfo);
};