#include "camera.h"
#include <iostream>

Camera::Camera(Vec3 origin, Vec3 orientation) : origin(origin), orientation(orientation) {
    vertical_fov = 90.0;
    view_up = Vec3(0, 1, 0);
    aspect_ratio = 1;
    transform(Vec3(), Vec3());
}

void Camera::transform(Vec3 translation, Vec3 rotation) {
    // Translate origin
    origin = origin + translation;

    orientation = get_rotated(orientation, rotation);
    view_target = origin + orientation;
    
    // Calculate viewport
    double theta = degrees_to_radians(vertical_fov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    Vec3 w = unit_vector(origin - view_target);
    Vec3 u = unit_vector(cross(view_up, w));
    Vec3 v = cross(w, u);

    viewport_horizontal = viewport_width * u;
    viewport_vertical = viewport_height * v;
    viewport_origin = origin - viewport_horizontal / 2 - viewport_vertical / 2 - w;
}

void Camera::translate(Vec3 translation) {
    transform(translation, Vec3());
}

void Camera::rotate(Vec3 rotation) {
    transform(Vec3(), rotation);
}

Ray Camera::get_ray(double s, double t) const {
    return Ray(origin, viewport_origin + s * viewport_horizontal + t * viewport_vertical - origin);
}

double degrees_to_radians(double degrees) {
    return degrees * 0.0174533;
}
