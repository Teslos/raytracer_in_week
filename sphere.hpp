#ifndef SPHEREH
#define SPHEREH

#include "hitable.hpp"
#include "material.hpp"
class sphere : public hitable {
    public:
        sphere() {}
        sphere(vec3 cen, float r, material *mat) : center(cen), radius(r), mat_ptr(mat)  {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        vec3 center;
        float radius;
        material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    rec.mat_ptr = this->mat_ptr;

    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - a*c;
    if(discriminant > 0) {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if(temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
class moving_sphere: public hitable {
    public:
        moving_sphere () {}
        moving_sphere (  vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) : 
            center0(cen0) , center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        vec3 center0, center1;
        vec3 center(float time) const;
        float time0, time1;
        float radius;
        material *mat_ptr;
};

vec3 moving_sphere::center(float time) const {
    return center0 +  (center1 - center0)* (time-time0)/(time1-time0);
}
bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - a*c;
    if(discriminant > 0) {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if(temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            return true;
        }
    }
    return false;
}
#endif


