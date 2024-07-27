#pragma once

#include <QImage>
#include "LibTrace/viewport.hpp"
#include "LibTrace/list.hpp"
#include "LibTrace/color.hpp"

class Scene {
   public:
    QSharedPointer<FigureList> list;
    QSharedPointer<Viewport> viewport;
    int samples_per_pixel;
    int max_depth;
    Scene() = default;
    Scene(QSharedPointer<FigureList> fl, QSharedPointer<Viewport> vp, int spp, int md) : list(fl), viewport(vp), samples_per_pixel(spp), max_depth(md) {}
    Color get_ray_color(int x, int y, int depth, Hittype hittype) const;
    QColor get_pixel_color(int x, int y, Hittype hittype) const;
    QImage render(Hittype hittype = SimpleHit);
};