#pragma once
#include "vec3.h"

struct Ray {
	Vec3 origin;
	Vec3 direction;

	Ray() {}
	Ray(Vec3 origin, Vec3 direction) :
		origin(origin), direction(direction) {}

	// Returns a point along a ray scaled by t
	Vec3 at(double t) {
		return origin + t * direction;
	}
};
