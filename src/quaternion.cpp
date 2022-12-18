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

Vec3 get_rotated(const Vec3& e_point, const Vec3& e_rotation) {
    Quaternion point = Quaternion(0, e_point.x, e_point.y, e_point.z);
    Quaternion rotation = quaternion_from_euler(e_rotation).normalized(); // * identity_quaternion();
    Quaternion inverse = rotation.inverse();

    // Perform the (passive) rotation
    point = rotation * point * inverse;
    //point = inverse * point * rotation;
    return Vec3(point.x, point.y, point.z);
}

Quaternion quaternion_from_euler(const Vec3& euler) {
    // TODO: Obviously cache trig functions?
    // Halved euler values
    double hx = euler.x / 2;
    double hy = euler.y / 2;
    double hz = euler.z / 2;
    //Quaternion
    return Quaternion(
        cos(hx)*cos(hy)*cos(hz) + sin(hx)*sin(hy)*sin(hz),
        sin(hx)*cos(hy)*cos(hz) - cos(hx)*sin(hy)*sin(hz),
        cos(hx)*sin(hy)*cos(hz) + sin(hx)*cos(hy)*sin(hz),
        cos(hx)*cos(hy)*sin(hz) - sin(hx)*sin(hy)*cos(hz)
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
