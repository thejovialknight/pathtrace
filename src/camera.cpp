#include "camera.h"

Camera::Camera(Vec3 position, Vec3 look_direction) : position(position), look_direction(look_direction) {
    transform(Vec3(), Vec3());
}

void Camera::transform(Vec3 translation, Vec3 rotation) {
    const double aspect_ratio = 1;
    const Vec3 up_direction = Vec3(0, 1, 0);
    const double fov = 90.0;

    position = position + translation;
    look_direction = look_direction + rotation;

    double theta = degrees_to_radians(fov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    Vec3 w = unit_vector(look_direction);
    Vec3 u = unit_vector(cross(up_direction, w));
    Vec3 v = cross(w, u);

    focal_length = 1.0;
    
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = position - horizontal / 2 - vertical / 2 - w;
}

void Camera::translate(Vec3 translation) {
    transform(translation, Vec3());
}

void Camera::rotate(Vec3 rotation) {
    transform(Vec3(), rotation);
}

Ray Camera::get_ray(double s, double t) const {
    return Ray(position, lower_left_corner + s * horizontal + t * vertical - position);
}

double degrees_to_radians(double degrees) {
    return degrees * 0.0174533;
}
