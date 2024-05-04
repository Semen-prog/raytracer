#ifndef STYLES_H
#define STYLES_H

#include "color.h"
#include "ray.h"
#include "geometry.h"

class Style {
 public:
    virtual ~Style() = default;
    virtual void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const {
        return;
    }
};

class Lamberitan: public Style {
 private:
    color albedo;
 public:
    Lamberitan(const color &a): albedo(a) {}
    void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        vec3 s_dir = rec.normal + random_vec();
        if (almost_zero(s_dir)) s_dir = rec.normal;
        scattered = ray(rec.p, s_dir);
        attenuation = albedo;
    }
};

class Metal: public Style {
 private:
    color albedo;
    double fizz; // radius of random circle
 public:
    Metal(const color &a, double f): albedo(a), fizz(f) {} 
    void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        vec3 s_dir = reflect(r_in.dir, rec.normal);
        s_dir = s_dir.unit() + random_vec() * fizz;
        scattered = ray(rec.p, s_dir);
        attenuation = albedo;
    }
};

double schlick_reflection(double coss, double ri) {
    auto r0 = (1 - ri) / (1 + ri);
    r0 = r0 * r0;
    return r0 + (1-r0) * pow(1 - coss, 5);
}

class Dielectric: public Style {
 private:
    double refractive_index;
    
 public:
    Dielectric(double ri): refractive_index(ri) {}
    void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        vec3 s_dir;
        double ri = rec.in_face ? refractive_index: 1 / refractive_index;
        if (!refract(r_in.dir, rec.normal, ri, s_dir) || schlick_reflection(std::min(-r_in.dir * rec.normal, 1 - eps), ri) > random_01()) {
            s_dir = reflect(r_in.dir, rec.normal);
        }
        scattered = ray(rec.p, s_dir);
        attenuation = color(1, 1, 1);
    }
};

#endif