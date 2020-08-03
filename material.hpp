#ifndef MATERIALH
#define MATERIALH
#include "ray.hpp"
#include "hitable.hpp"
struct hit_record;
class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};
class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
        vec3 albedo;
};
class metal : public material {
    public:
        metal(const vec3& a, float f) :  albedo(a) {if(f < 1) fuzz = f; else fuzz = 1;}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
        vec3 albedo;
        float fuzz;
};
class dielectric : public material {
    public:
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
    float ref_idx;
};
#endif
