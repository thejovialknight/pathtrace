#include "vec3.h"

Vec3 operator+(const Vec3& a, const Vec3& b) {
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 operator*(const Vec3& a, const double t) {
	return Vec3(a.x * t, a.y * t, a.z * t);
}

Vec3 operator*(const double t, const Vec3& a) {
	return a * t;
}

Vec3 operator/(const Vec3& a, const double t) {
	return (1 / t) * a;
}
