#pragma once
#include "vec3.h"

struct Camera {
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    double focal_length;

    Camera() {
        double viewport_height = 2.0;
        double viewport_width = 2.0;
        focal_length = 1.0;
        
        origin = Vec3(0, 0, 0);
        horizontal = Vec3(viewport_width, 0, 0);
        vertical = Vec3(0, viewport_height, 0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
    }

    void translate(Vec3 translation) {
        origin = origin + translation;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
    }

    Ray get_ray(double u, double v) const {
        return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
};
