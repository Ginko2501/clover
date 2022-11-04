#ifndef MATERIAL_H
#define MATERIAL_H

#include "../clover.h"

struct hit_record;

// struct scatter_record {
//     ray r_out;
//     color reflectance;
//     double pdf;
//     double brdf;
// };

class material {
    public:
        virtual void scatter(ray& r_in, scatter_record& s_rec) {}
    
    public:
        std::string name;
        color emit;
        color reflectance;
};

class light : public material {
    public:
        light() {
            name = "Light";
            emit = color(5, 5, 5);
        }
        light(const color& c) {
            name = "Light";
            emit = c;
        }
};

class lambertian : public material {
    public:
        lambertian() {name = "Lambertian";}
        lambertian(const color& a) {
            name = "Lambertian";
            emit = color(0, 0, 0);
            reflectance = a;
        }
        lambertian(const color& a, const color& e) {
            name = "Lambertian";
            emit = e;
            reflectance = a;
        }

        virtual void scatter(
            ray& r_in, scatter_record& rec
        ) override {
            auto n = rec.obj->normal(rec.origin);
                 n = dot(r_in.dir, n)>0 ? -n:n;
            auto scatter_direction = n + random_unit_vector();

            // Catch degenerate scatter direction
            if (is_zero(scatter_direction))
                scatter_direction = n;

            rec.r_out = ray(rec.origin, unit_vector(scatter_direction));
            rec.r_out.orig += epsilon * rec.r_out.dir;
            rec.pdf = 0.5 / pi;
            rec.brdf = 1.0 / pi; 
        }
};


class metal : public material {
    public:
        metal(const color& a, double f) {
            name = "Metal";
            emit = color(0, 0, 0);
            reflectance = a;
            fuzz = f < 1 ? f : 1;
        }

        virtual void scatter(
            ray& r_in, scatter_record& rec
        ) override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.obj->normal(rec.origin));
            rec.r_out = ray(rec.origin, reflected + fuzz*random_in_unit_sphere());
            //return (dot(scattered.direction(), rec.normal()) > 0);
        }

    public:
        double fuzz;
};

// do not declare as material
auto material_light = light();
auto material_light_dim = light(color(1, 1, 1));
auto material_light_bright = light(color(15, 15, 15));

auto material_ground = lambertian(color(0.1, 0.1, 0.0));
auto material_center = lambertian(color(0.1, 0.2, 0.5));

auto lambertian_red = lambertian(color(0.65, 0.05, 0.05));
auto lambertian_green = lambertian(color(0.12, 0.45, 0.15));
auto lambertian_white = lambertian(color(0.73, 0.73, 0.73));

#endif