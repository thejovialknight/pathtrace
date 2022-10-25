#pragma once
#include "vec3.h"

struct HitInfo {
    bool hit = false;
    Vec3 point;
    Vec3 normal; // Always faces against ray as opposed to always facing out from surface
    double t;
    bool outside_surface;

    HitInfo() {}
    HitInfo(bool hit) : hit(hit) {}

    // Calculates outside_surface and the normal from outward_normal
    HitInfo(bool hit, Vec3 point, const Vec3& outward_normal, const Ray& ray, double t) : hit(hit), point(point), t(t) {
        outside_surface = dot(ray.direction, outward_normal) < 0;
        normal = outside_surface ? outward_normal : -outward_normal; // Ternary sets normal based on condition
    }
};
