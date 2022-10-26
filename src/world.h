#pragma once
#include "sphere.h"
#include <vector>
#include "camera.h"

struct World {
    Camera camera;
    std::vector<Sphere> spheres;

    void translate(Vec3 translation) {
        for(Sphere& sphere : spheres) {
            sphere.center = sphere.center + translation;
        }
    }
};
