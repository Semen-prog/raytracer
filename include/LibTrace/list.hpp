#pragma once

#include <QSharedPointer>
#include "LibTrace/shapes.hpp"
#include "LibTrace/materials.hpp"
#include "LibTrace/textures.hpp"


struct figure_record {
    Color a;
    Ray s;
    bool r;
};

class Figure {
   protected:
    QSharedPointer<Shape> shape;
    QSharedPointer<Material> material;
    QSharedPointer<Texture> texture;

   public:
    Figure() = default;
    Figure(QSharedPointer<Shape> s, QSharedPointer<Material> m, QSharedPointer<Texture> t) : shape(s), material(m), texture(t) {}
    Bbox bbox() const;
    bool hit(const Ray &r_in, Interval &zone, figure_record &fr) const;
};

enum Hittype {
    SimpleHit,
    HardHit
};

class FigureList {
   private:
    int n;
    std::vector<Bbox> t;
    std::vector<Figure> figs;
    void build(int p, int l, int r);
    bool simple_hit(const Ray &r_in, Interval &zone, figure_record &fr) const;
    bool hard_hit(int p, int l, int r, const Ray &r_in, Interval &zone, figure_record &fr) const;

   public:
    FigureList() {
        n = 0;
        figs = {};
    }
    void add(QSharedPointer<Shape> shape, QSharedPointer<Material> material, QSharedPointer<Texture> texture);
    void build();
    bool hit(const Ray &r_in, figure_record &fr, Hittype hittype) const;
};