#include "quaternion.h"

double Quaternion::magnitude() {
    return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::normalized() {
    double m = magnitude();
    return Quaternion(w / m, x / m, y / m, z / m);
}

Quaternion Quaternion::inverse() {
    return Quaternion(w, -x, -y, -z);
}

Vec3 get_camera_orientation(double pitch, double yaw) {
    Quaternion q_pitch = quaternion_from_euler(Vec3(pitch, 0, 0));
    Quaternion q_yaw = quaternion_from_euler(Vec3(0, yaw, 0));
    Quaternion initial = quaternion_from_euler(Vec3(0,0,1));
    Quaternion rotation = q_pitch * initial * q_yaw;
    Quaternion orientation = rotation * initial * rotation.inverse();
    return unit_vector(Vec3(orientation.x, orientation.y, orientation.z));
}

Vec3 new_get_rotated(const Vec3& e_point, const Vec3& e_rotation) {
    Quaternion point = identity_quaternion();
    Quaternion rotation = quaternion_from_euler(e_rotation).normalized(); // * identity_quaternion();
    Quaternion inverse = rotation.inverse();

    // Perform the (passive) rotation
    point = rotation * point * inverse;
    //point = inverse * point * rotation;
    return unit_vector(Vec3(point.x, point.y, point.z));
}

Vec3 get_rotated_from_axis_angle(Vec3& e_point, Vec3& axis, double angle) {
    Quaternion point = Quaternion(0, e_point.x, e_point.y, e_point.z);
    Quaternion rotation(
        cos(angle/2),
        axis.x * sin(angle/2),
        axis.y * sin(angle/2),
        axis.z * sin(angle/2)
    );
    Quaternion inverse = rotation.inverse();

    // Perform the (passive) rotation
    //point = rotation * point * inverse;
    point = inverse * point * rotation;
    //return euler_from_quaternion(point);
    return Vec3(point.x, point.y, point.z);
}

Vec3 get_rotated(const Vec3& e_point, const Vec3& e_rotation) {
    Quaternion point = Quaternion(0, e_point.x, e_point.y, e_point.z);
    Quaternion rotation = quaternion_from_euler(e_rotation) * identity_quaternion();
    Quaternion inverse = rotation.inverse();

    // Perform the (passive) rotation
    //point = rotation * point * inverse;
    point = inverse * point * rotation;
    //return euler_from_quaternion(point);
    return Vec3(point.x, point.y, point.z);
}

Quaternion quaternion_from_euler(const Vec3& euler) {
    // roll/pitch/yaw=x/y/z
    double cr = cos(euler.x / 2);
    double cp = cos(euler.y / 2);
    double cy = cos(euler.z / 2);
    double sr = sin(euler.x / 2);
    double sp = sin(euler.y / 2);
    double sy = sin(euler.z / 2);
    double cpcy = cp * cy;
    double spsy = sp * sy;
    return Quaternion(
        cr * cpcy + sr * spsy, 
        sr * cpcy - cr * spsy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy
    );
}

// From wikipedia page "Conversion between quaternions and Euler angles"
Vec3 euler_from_quaternion(const Quaternion& q) {
    Vec3 euler;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    euler.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        euler.y = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        euler.y = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    euler.z = std::atan2(siny_cosp, cosy_cosp);

    return euler;
}

Quaternion identity_quaternion() {
    return Quaternion(1, 0, 0, 0);
}

// WARNING: Quaternion multiplication is not communicative!
Quaternion operator*(const Quaternion& a, const Quaternion& b) {
    return Quaternion(
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
    );
}
