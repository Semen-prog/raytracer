#include "LibTrace/tracing.hpp"

Bbox Figure::bbox() const {
    return shape->bbox();
}
bool Figure::hit(const Ray &r_in, Interval &zone, figure_record &fr) const {
    shape_record rd;
    if (!shape->hit(r_in, zone, rd)) return false;
    fr.a = texture->value(rd);
    fr.r = material->scatter(r_in, rd, fr.s);
    return true;
}

void FigureList::build(int p, int l, int r) {
    if (l + 1 == r) {
        t[p] = figs[l].bbox();
        return;
    }
    int mid = (l + r) / 2;
    int mode = randint(0, 3);
    if (mode == 0) {
        std::sort(figs.begin() + l, figs.begin() + r, [](Figure &b1, Figure &b2) {
            return b1.bbox().x.left < b2.bbox().x.left;
        });
    } else if (mode == 1) {
        std::sort(figs.begin() + l, figs.begin() + r, [](Figure &b1, Figure &b2) {
            return b1.bbox().y.left < b2.bbox().y.left;
        });
    } else {
        std::sort(figs.begin() + l, figs.begin() + r, [](Figure &b1, Figure &b2) {
            return b1.bbox().z.left < b2.bbox().z.left;
        });
    }
    build(2 * p + 1, l, mid);
    build(2 * p + 2, mid, r);
    t[p] = t[2 * p + 1] | t[2 * p + 2];
}
bool FigureList::simple_hit(const Ray &r_in, Interval &zone, figure_record &fr) const {
    bool flag = false;
    for (int i = 0; i < n; ++i) {
        if (figs[i].hit(r_in, zone, fr)) flag = true;
    }
    return flag;
}
bool FigureList::hard_hit(int p, int l, int r, const Ray &r_in, Interval &zone, figure_record &fr) const {
    if (n == 0) return false;
    if (l + 1 == r) return figs[l].hit(r_in, zone, fr);
    if (!t[p].hit(r_in, zone)) return false;
    int mid = (l + r) / 2;
    return hard_hit(2 * p + 1, l, mid, r_in, zone, fr) | hard_hit(2 * p + 2, mid, r, r_in, zone, fr);
}
void FigureList::add(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material, std::shared_ptr<Texture> texture) {
    ++n;
    figs.emplace_back(shape, material, texture);
}
void FigureList::build() {
    if (n == 0) return;
    t.resize(4 * n);
    build(0, 0, n);
}
bool FigureList::hit(const Ray &r_in, figure_record &fr, Hittype hittype) const {
    Interval zone(0, inf);
    if (hittype == SimpleHit) return simple_hit(r_in, zone, fr);
    return hard_hit(0, 0, n, r_in, zone, fr);
}