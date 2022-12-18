#include "camera.h"
#include <iostream>

Camera::Camera(Vec3 origin, Vec3 orientation) : origin(origin), orientation(orientation) {
    // Angle stuff 
    yaw = 0;
    pitch = 0;

    vertical_fov = 90.0;
    view_up = Vec3(0, 1, 0);
    aspect_ratio = 1;
    transform(Vec3(), Vec3());
}

void Camera::transform(Vec3 translation, Vec3 rotation) {
    double rotate_speed = 0.4;
    if(!rotation.near_zero()) {
        yaw += rotation.x * rotate_speed;
        pitch += rotation.y * rotate_speed;
        if(yaw > 6.28) { yaw -= 6.28; }
        else if(yaw < 0) { yaw += 6.28; }
        if(pitch > 1.5) { pitch = 1.5; }
        else if(pitch < -1.5) { pitch = -1.5; }
                
        orientation = Vec3(0,0,1);
        Vec3 r(0, -yaw, pitch);
        
        // FUCKERY
        // Rotate by -yaw? (2)
        Vec3 yaw = Vec3(
            orientation.x*cos(r.y)+orientation.z*sin(r.y),
            orientation.y,
            -orientation.x*sin(r.y)+orientation.z*cos(r.y)
        );
        // Rotate by pitch (3)
        Vec3 pitch = Vec3(
            orientation.x,
            orientation.y*cos(r.z)-orientation.z*sin(r.z),
            orientation.y*sin(r.z)+orientation.z*cos(r.z)
        );
        orientation = Vec3(yaw.x, pitch.y, yaw.z);
        /* Rotate by roll? (1)
        orientation = Vec3(
            orientation.x*cos(r.x)-orientation.y*sin(r.x),
            orientation.x*sin(r.x)+orientation.y*cos(r.x),
            orientation.z
        );
        //*/

        /* ORIGINAL
        // Rotate by yaw
        orientation = Vec3(
            orientation.x*cos(r.x)-orientation.y*sin(r.x),
            orientation.x*sin(r.x)+orientation.y*cos(r.x),
            orientation.z
        );
        // Rotate by pitch
        orientation = Vec3(
            orientation.x*cos(r.y)+orientation.z*sin(r.y),
            orientation.y,
            -orientation.x*sin(r.y)+orientation.z*cos(r.y)
        );
        // Rotate by roll
        orientation = Vec3(
            orientation.x,
            orientation.y*cos(r.z)-orientation.z*sin(r.z),
            orientation.y*sin(r.z)+orientation.z*cos(r.z)
        );
        //*/
    }

    // Translate origin
    double move_speed = 1;
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

