#pragma once
#include <cmath>

struct Vec3 {
	double x = 0;
	double y = 0;
	double z = 0;

	Vec3() {}
	Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

	double length() const { return sqrt(length_squared()); }
	double length_squared() const { return (x * x) + (y * y) + (z * z); }

	Vec3 operator-() const { return Vec3(-x, -y, -z); }
};

Vec3 operator+(const Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a, const Vec3& b);
Vec3 operator*(const Vec3& a, const double t);
Vec3 operator*(const double t, const Vec3& a);
Vec3 operator/(const Vec3& a, const double t);

inline double dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

inline Vec3 cross(const Vec3& a, const Vec3& b) {
	return Vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

inline Vec3 unit_vector(const Vec3 vec) { return vec / vec.length(); }
