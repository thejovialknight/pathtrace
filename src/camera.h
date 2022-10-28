// Hello, some notes:
// This way of dealing with the camera and the tracing and shit is obviously fucked.
// We are going to need a way to set up the camera in a sane manner, the way one might expect.
//
// To this end, we will finally get a little serious here and make a Transform struct to be applied to both cameras and objects like spheres.
//
// If it ends up being more performant to precompute some sort of values in camera so that the tracer doesn't have to every ray or pixel or whatever, just do that in the tracer. The camera shouldn't know about any of this.
//
// This get_ray function is a bit mysterious at the moment. It takes in a 2d screen space coordinate (pixels or scaled value?) and returns a ray fired forward from that screen position, at least presumably. Therefore, it should really be renamed, forward_ray_from_screen_point or something like that. Try to obviously figure out a good name for it based on whatever it actually does.
//
// Get table.h up in here and figure that out. Back to main.h for more notes on all that

#pragma once
#include <math.h>

#include "vec3.h"
#include "ray.h"

struct Camera {
    // Member variables
    Vec3 position;
    Vec3 look_direction;
    Vec3 fov; // vertical

    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    double focal_length;

    // Constructor
    Camera(Vec3 position, Vec3 look_direction);

    // Methods
    void transform(Vec3 translation, Vec3 rotation);
    void translate(Vec3 translation);
    void rotate(Vec3 rotation);
    Ray get_ray(double u, double v) const;
};

// Converts degrees into radians
double degrees_to_radians(double degrees);
