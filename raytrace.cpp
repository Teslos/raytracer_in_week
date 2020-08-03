#include<iostream>
#include "sphere.hpp"
#include "camera.hpp"
#include "hitablelist.hpp"
#include "material.hpp"
#include <float.h>
#include <stdio.h>


const vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if ( world->hit(r,0.0,MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r,rec, attenuation,scattered)) {
            return attenuation * color(scattered, world, depth  + 1);
        } else {
            return vec3(0,0,0);
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return vec3(1.0,1.0,1.0)*(1.0-t) + vec3(0.5,0.7,1.0)*t;
    }
}

hitable *random_scene() {
    int n = 300;
    hitable **list = new hitable* [n+1];
    list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5,0.5,0.5)));
    int i = 1;
    for(int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a + 0.9*drand48(),0.2, b+0.9*drand48());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) { // diffuse
                    vec3 *mat = new vec3(drand48()*drand48(),drand48()*drand48(), drand48()*drand48());
                    list[i++] = new sphere(center, 0.2, new lambertian(*mat));
                }
                else if (choose_mat < 0.95) { // meta1
                    list[i++] = new sphere(center, 0.2, 
                        new metal(vec3(0.5*(1+drand48()), 0.5*(1+drand48()),0.5*(1+drand48())), 0.5*drand48()));
                }
                else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }    
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4,0.2,0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7,0.6,0.5),0.0));
    return new hitable_list(list, i);
}   

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0,-1.0,-1.0);
    vec3 horizontal(4.0,0.0,0.0);
    vec3 vertical(0.0,2.0,0.0);
    vec3 origin(0.0,0.0,0.0);
    hitable *list[4];
    float R = cos(M_PI/4);
    list[0] = new sphere(vec3(-R,0,-1),R, new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere(vec3(R,0.0,-1),R, new lambertian(vec3(0.8,0.6,0.2)));
    list[2] = new sphere(vec3(0,-100.0-R,-1),100, new lambertian(vec3(0.8,0.8,0.0)));
    list[3] = new sphere(vec3(-3.0*R,0.0,-1), R, new metal(vec3(0.8,0.6,0.2),0.1));
    //list[3] = new sphere(vec3(-2*R,0,-1), R, new dielectric(1.5));
    //hitable *world = new hitable_list(list,4);
    hitable *world = random_scene();
    vec3 lookfrom(3,3,2);
    vec3 lookat(0,0,-1);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperature = 1.0;
    camera cam(lookfrom, lookat, vec3(0,1,0),20, float(nx)/float(ny),aperature,dist_to_focus);
    for(int j = ny - 1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            vec3 col(0,0,0);
            for (int s=0; s < ns; s++) {

                float u = float(i+drand48()) / float(nx);
                float v = float(j+drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r,world,0); 
            }
            col /= float(ns);
            int ir = int(255.99*sqrt(col[0]));
            int ig = int(255.99*sqrt(col[1]));
            int ib = int(255.99*sqrt(col[2]));

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

