#ifndef TRACING_H
#define TRACING_H

#include <math.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#ifdef DEBUG
#include <chrono>
#include <iostream>
#endif // DEBUG

extern std::mt19937 rnd;
extern std::uniform_real_distribution<long double> ud;

int randint(int l, int r);
long double uniform(long double l, long double r);

constexpr long double eps = 1e-6;
constexpr long double inf = 1'000'000'000'000'000'000;

#define radian(x) ((x) / 180.0 * (long double)M_PI)

class Vector {
 public:
    long double x, y, z;
    Vector() = default;
    Vector(long double _x, long double _y, long double _z): x(_x), y(_y), z(_z) {}
    Vector normalized() const;
    long double length() const;
    static long double dotProduct(const Vector &v, const Vector &u);
    static Vector crossProduct(const Vector &v, const Vector &u);
    static Vector normal(const Vector &v, const Vector &u);
};

Vector operator+(const Vector &v, const Vector &u);
Vector operator*(const Vector &v, const Vector &u);
Vector operator*(const Vector &v, long double x);
Vector operator*(double x, const Vector &v);
Vector operator-(const Vector &v);
Vector operator-(const Vector &v, const Vector &u);
Vector operator/(const Vector &v, long double x);

using Point = Vector;
using Color = Vector;

Vector random_unit();

Color gamma_correction(const Color &col, long double gamma);
std::tuple<int, int, int> correct(const Color &col);
bool almost_black(const Color &col);

class Ray {
   public:
    Point start;
    Vector dir;
    long double time;
    Ray() = default;
    Ray(const Point &s, const Vector &d) : start(s), dir(d.normalized()), time(uniform(0, 1)) {}
    Ray(const Point &s, const Vector &d, long double t) : start(s), dir(d.normalized()), time(t) {}
    long double ptime(const Point &p) const;
    Point operator()(long double t) const;
};

class Interval {
   public:
    long double left, right;
    Interval() = default;
    Interval(long double l, long double r) : left(l), right(r) {}
    Interval operator&(const Interval &i) const;
    Interval operator|(const Interval &i) const;
    Interval operator+(long double x) const;
    bool contains(long double x) const;
    bool surrounds(long double x) const;
    Interval hit(long double start, long double dir) const;
    bool empty() const;
};

class Bbox {
   public:
    Interval x, y, z;
    Bbox() = default;
    Bbox(const Interval &_x, const Interval &_y, const Interval &_z) : x(_x), y(_y), z(_z) {}
    Bbox operator+(const Vector &v) const;
    Bbox operator|(const Bbox &b) const;
    bool hit(const Ray &r, const Interval &zone) const;
};

struct shape_record {
    Point p;
    Vector n;
    long double t;
    bool ifc;
    long double x, y;
};

class Shape {
   protected:
    Vector move;
    virtual Bbox base_bbox() const = 0;

   public:
    Bbox bbox() const;
    virtual bool hit(const Ray &r, Interval &zone, shape_record &rd) const = 0;
};

class Material {
   public:
    virtual bool scatter(const Ray &r_in, const shape_record &rd, Ray &scattered) const = 0;
};

Vector reflect(const Vector &d, const Vector &n);
Vector refract(const Vector &d, const Vector &n, long double ref);

class Texture {
   public:
    virtual Color value(const shape_record &rd) const = 0;
};

struct figure_record {
    Color a;
    Ray s;
    bool r;
};

class Figure {
   protected:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<Texture> texture;

   public:
    Figure() = default;
    Figure(std::shared_ptr<Shape> s, std::shared_ptr<Material> m, std::shared_ptr<Texture> t) : shape(s), material(m), texture(t) {}
    Bbox bbox() const;
    bool hit(const Ray &r_in, Interval &zone, figure_record &fr) const;
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
    const static int SimpleHit = 0;
    const static int HardHit = 1;
    FigureList() {
        n = 0;
        figs = {};
    }
    void add(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material, std::shared_ptr<Texture> texture);
    void build();
    bool hit(const Ray &r_in, figure_record &fr, int hittype) const;
};

Vector random_unit_disk();

class Viewport {
   private:
    Vector viewport_delta_x, viewport_delta_y;
    Point upper_left;
    Point center;
    long double offset_x, offset_y;
    long double defocus_rad;
    Point random_orig() const;
    Point get_pixel(int x, int y) const;

   public:
    int pixel_width, pixel_height;
    Viewport() = default;
    Viewport(
        int pw,               // pixel width
        int ph,               // pixel height
        long double fov,      // vertical field of view
        long double ox,       // offset for OX
        long double oy,       // offset for OY
        Point lookfrom,       // look from (camera position)
        Point lookat,         // look at (image center)
        Vector dir_up,        // up from camera
        long double da        // defocus angle
    ) {
        pixel_width = pw;
        pixel_height = ph;
        center = lookfrom;
        long double focal_length = (lookat - lookfrom).length();
        long double viewport_height = tan(fov / 2) * focal_length * 2;
        long double viewport_width = viewport_height * (double)pixel_width / (double)pixel_height;
        Vector front = (lookat - lookfrom).normalized();
        Vector right = Vector::crossProduct(front, dir_up).normalized();
        Vector up = Vector::crossProduct(right, front).normalized();
        Vector viewport_x = right * viewport_width;
        Vector viewport_y = (-up) * viewport_height;
        viewport_delta_x = viewport_x / pixel_width;
        viewport_delta_y = viewport_y / pixel_height;
        upper_left = lookat - viewport_x / 2 - viewport_y / 2;
        offset_x = ox;
        offset_y = oy;
        defocus_rad = focal_length * tan(da / 2);
    }
    Ray get_ray(int x, int y) const;
};

class Scene {
   public:
    std::shared_ptr<FigureList> list;
    std::shared_ptr<Viewport> viewport;
    int samples_per_pixel;
    int max_depth;
    Scene() = default;
    Scene(std::shared_ptr<FigureList> fl, std::shared_ptr<Viewport> vp, int spp, int md) : list(fl), viewport(vp), samples_per_pixel(spp), max_depth(md) {}
    Color get_ray_color(int x, int y, int depth, int hittype) const;
    std::tuple<int, int, int> get_pixel_color(int x, int y, int hittype) const;
    std::vector<std::vector<std::tuple<int, int, int>>> render(int hittype = FigureList::SimpleHit);
};

#endif // TRACING_H