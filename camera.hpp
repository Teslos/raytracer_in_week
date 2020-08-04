#ifndef CAMERAH
#define CAMERAH

#include "ray.hpp"
vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = vec3(drand48(), drand48(),0)*2.0-vec3(1,1,0);
    }while(dot(p,p) >= 1.0);
    return p;
}

class camera {
    public:
        camera(vec3 lookfrom, vec3 lookat,vec3 vup,  float vfov, float aspect, float aperature, float focus_dist, float t0, float t1) {
            time0 = t0; time1 = t1;
            lens_radius = aperature / 2;
            float theta = vfov * M_PI/180.0;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w,u);
            lower_left_corner = origin - u*half_width * focus_dist-v*half_height*focus_dist-w*focus_dist;
            horizontal = u*2*half_width*focus_dist; 
            vertical =  v*2*half_height*focus_dist;
            //origin = vec3(0.0,0.0,0.0);
        }
        ray get_ray(float s, float t) {
            vec3 rd = random_in_unit_disk() * lens_radius;
            vec3 offset = u*rd.x() + v*rd.y(); 
            float time = time0 + drand48() *(time1-time0);
            return ray(origin+offset, lower_left_corner + horizontal * s + vertical*t -origin-offset, time); }
        vec3 u, v, w;
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        float time1, time0;
        float lens_radius;
};
#endif
