#include "camera.h"
#include <iostream>

Camera::Camera(Vec3 origin, Vec3 orientation) : origin(origin), orientation(orientation) {
    vertical_fov = 90.0;
    view_up = Vec3(0, 1, 0);
    aspect_ratio = 1;
    transform(Vec3(), Vec3());
}

void Camera::transform(Vec3 translation, Vec3 rotation) {
    double rotate_speed = 0.2;
    if(!rotation.near_zero()) {
        orientation = get_rotated(orientation, unit_vector(rotation) * rotate_speed);
        std::cout << "Orientation " << orientation.x << "," << orientation.y << "," << orientation.z << "," << std::endl;
    }

    // Translate origin
    double move_speed = 4;
    if(!translation.near_zero()) {
        Vec3 right = cross(orientation, view_up);
        Vec3 forward = cross(right, view_up);
        Vec3 movement = (right * translation.x) + (forward * translation.y);
        std::cout << "movement: " << movement.x << "," << movement.y << "," << movement.z << "," << std::endl;
        origin = origin + movement * move_speed;
    }

    view_target = origin + orientation;
    
    // Calculate viewport
    double theta = degrees_to_radians(vertical_fov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    // New code
    Vec3 w = unit_vector(origin - view_target); // originally called w
    Vec3 u = unit_vector(cross(view_up, w)); // called u. 
    Vec3 v = cross(w, u); // called v
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
