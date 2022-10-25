#pragma once
#include "vec3.h"
#include "ray.h"
#include "hitinfo.h"

struct Sphere {
    Vec3 center = Vec3(0, 0, 0);
    double radius = 1;

    Sphere() {}
    Sphere(Vec3 center, double radius) : center(center), radius(radius) {}

    HitInfo hit(const Ray& ray, double t_min, double t_max) const;
};
