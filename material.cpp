#include "material.hpp"
#include "hitable.hpp"

vec3 random_in_unit_sphere() { 
    vec3 p;
    do {
        p = vec3(drand48(), drand48(), drand48())*2.0 - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);
    return p;
}

bool lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p, target-rec.p);

    attenuation = albedo;
    return true;
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v -  n * 2 * dot(v,n);
}

bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + random_in_unit_sphere()*fuzz);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1-dt*dt);
    if (discriminant > 0) {
        refracted = (v-n*dt)*ni_over_nt - n * sqrt(discriminant);
        return true;
    }
    else
        return false;
}

float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0 * r0;
    return r0 * (1-r0) * pow((1-cosine),5);
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 outward_normal;
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = vec3(1.0,1.0,0.0);
    vec3 refracted;
    float reflect_prob;
    float cosine;
    if (dot(r_in.direction(), rec.normal) > 0) {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }
    else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }
    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
        reflect_prob =schlick(cosine, ref_idx);
    }
    else {
        scattered = ray(rec.p, reflected);
        reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
        scattered = ray(rec.p, reflected);
    }
    else {
       scattered = ray(rec.p, refracted);
    } 
    return true;
}

