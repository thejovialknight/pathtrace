#pragma once
#include "vec3.h"
#include "ray.h"
#include "hitinfo.h"
#include "world.h"
#include "sphere.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <limits>

Vec3 color_from_ray(const Ray& ray, const World& world) {
    double t_max = std::numeric_limits<double>::infinity();
    HitInfo hit_info(false); // Stores the closest hit

    // For different shapes just need more for loops, continuing to populate hit info based on proximity
    for(const Sphere& sphere : world.spheres) {
        if(hit_info.hit) {
            t_max = hit_info.t;
        }
        HitInfo temp_info = sphere.hit(ray, 0, t_max);
        if(temp_info.hit) {
            hit_info = temp_info;
        }
    }

    if(hit_info.hit) {
        return 0.5 * (hit_info.normal + Vec3(1, 1, 1));
    }

    Vec3 unit_direction = unit_vector(ray.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
}

void render(SDL_Renderer* renderer, World& world, Vec3 cam_velocity) {
	// Screen settings
	const int image_width = 128;
	const int image_height = 128;
	const int pixel_scalar = 8;

	// Camera settings
	const double viewport_height = 2.0;
	const double viewport_width = 2.0;
    const double focal_length = 1.0;
	
	//
	const Vec3 origin = Vec3(0, 0, 0);
	const Vec3 horizontal = Vec3(viewport_width, 0, 0);
	const Vec3 vertical = Vec3(0, viewport_height, 0);
	const Vec3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

    // Transform world to simulate cam movement
    world.translate(cam_velocity);

	// Render image
	SDL_RenderClear(renderer);
	for(int y = image_height - 1; y >= 0; --y) {
		for(int x = 0; x < image_width; ++x) {
			double u = double(x) / (image_width - 1);
			double v = double(y) / (image_height - 1);
			Ray ray(
				origin, 
				lower_left_corner + u * horizontal + v * vertical - origin
			);
			Vec3 pixel_color = color_from_ray(ray, world) * 255.0;
			SDL_SetRenderDrawColor(renderer, (int)pixel_color.x, (int)pixel_color.y, (int)pixel_color.z, 255);

			// Setup scaled rect and draw
			SDL_Rect rect;
			rect.x = x * pixel_scalar;
			rect.y = y * pixel_scalar;
			rect.w = pixel_scalar;
			rect.h = pixel_scalar;
			SDL_RenderFillRect(renderer, &rect);
			//SDL_RenderPresent(renderer);
		}
	}
	SDL_RenderPresent(renderer);
}

// ALL OLD CODE!!

// New code post section 6.2
// Simplified quadratic formula by halving b 
// and also by using the logic that vectors dotted with themselves are equal to the squared length of the vector.
double hit_sphere(const Vec3& sphere_center, double sphere_radius, const Ray& ray) {
	Vec3 oc = ray.origin - sphere_center; 
	double a = ray.direction.length_squared(); // A vector dotted with itself is equal to the squared length of the vector. Hence this from dot(r.dir, r.dir)
	double half_b = dot(oc, ray.direction); // Halving b in the case of b = 2h (the factor of 2 in 2*dot(oc,r.dir)) simplifies quad. form to (-h +- sqrt(h^2) - ac) / a
	double c = oc.length_squared() - sphere_radius * sphere_radius; // Using the same logic as the new calculation for a
	double discriminant = half_b * half_b - a * c; // Simplified discriminant calculation from halving b

    if(discriminant < 0) { // if we didn't hit the sphere
        return -1.0;
    }
    else { // if we hit the sphere
        return (-half_b - sqrt(discriminant)) / (a); // 2.0 * a simplified to just a because we halved b
    }
}

// Before simplification in 6.2. Go there and review whats going on
// Quad. formula = (-b +- sqrt(b^2 - 4ac)) / 2a
// b^2 - 4ac = the discriminant
double old_hit_sphere(const Vec3& sphere_center, double sphere_radius, const Ray& ray) {
	Vec3 oc = ray.origin - sphere_center; // difference between ray start and sphere center?
	
    // Creating the discriminant of the quadratic formula (b^2 - 4ac)
    // First we populate the a, b, and c values with the appropriate math derived from all the horseshit
    double a = dot(ray.direction, ray.direction);
	double b = 2.0 * dot(oc, ray.direction);
	double c = dot(oc, oc) - sphere_radius * sphere_radius;
    // Then we do a part for part
	double discriminant = b * b - 4 * a * c; // b^2 - 4ac

    if(discriminant < 0) { // if we didn't hit the sphere
        return -1.0;
    }
    else { // if we hit the sphere
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}
// Linearly blends white and blue depending on
// the height of the y coordinate after scaling the ray direction to unit length.
Vec3 ray_color(const Ray& ray) {
    double t = hit_sphere(Vec3(0, 0, -1), 0.5, ray);
    if(t > 0.0) { // If hit sphere
        Vec3 n = unit_vector(ray.at(t) - Vec3(0, 0, -1));
        return 0.5 * Vec3(n.x + 1, n.y + 1, n.z + 1);
    }

    // If didn't hit sphere
	Vec3 unit_direction = unit_vector(ray.direction); // scale to unit length
	t = 0.5 * (unit_direction.y + 1.0); // Set scalar for lerp gradient

	// Standard lerp of the form L(t) = (1 - t) * start + t * end
	// First vec3 is the color white and the second is blue
	return (1.0 - t) * Vec3(0.1, 0.1, 0.1) + t * Vec3(0.5, 0.2, 0.2);
}

