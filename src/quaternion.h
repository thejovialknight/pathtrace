/* QUATERNIONS
Rotation via quaternions is as such.
We generate a temporary quaternion, which will represent the change in rotation.
If we multiply this with the "permenant" quaternion, the result will be a quaternion which has been changed by the rotation described in the temporary quaternion.
*/

#pragma once
#include <math.h>
#include "vec3.h"

struct Quaternion {
    double w;
    double x;
    double y;
    double z;

    Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

    double magnitude();
    Quaternion normalized();
    Quaternion inverse();
};

Quaternion operator*(const Quaternion& a, const Quaternion& b);
Vec3 get_camera_orientation(double pitch, double yaw);
Vec3 get_rotated_from_axis_angle(Vec3& point, Vec3& axis, double angle);
Vec3 get_rotated(const Vec3& e_point, const Vec3& e_rotation);
Quaternion identity_quaternion();
Quaternion quaternion_from_euler(const Vec3& euler);
Vec3 euler_from_quaternion(const Quaternion& q);
