#pragma once
#include "vec3.h"
#include <math.h>

// Can be used to describe any general material
// Right now its being used by pointer in these classes, but should be table optimally.
struct Material {
    Vec3 albedo;
    double metallicity = 0.0;

    Material() {}
    Material(Vec3 albedo) : albedo(albedo) {}
    Material(Vec3 albedo, double metallicity) : albedo(albedo), metallicity(metallicity) {}
};
