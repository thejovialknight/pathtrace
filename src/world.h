#pragma once
#include "sphere.h"
#include <vector>

struct World {
    std::vector<Sphere> spheres;

    void translate(Vec3 translation) {
        for(Sphere& sphere : spheres) {
            sphere.center = sphere.center + translation;
        }
    }
};
