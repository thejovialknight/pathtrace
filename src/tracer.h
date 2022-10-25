#pragma once
#include "vec3.h"
#include "ray.h"
#include <SDL2/SDL.h>

bool hit_sphere(const Vec3& sphere_center, double sphere_radius, const Ray& ray) {
	Vec3 oc = ray.origin - sphere_center; // difference between ray start and sphere center?
	double a = dot(ray.direction, ray.direction);
	double b = 2.0 * dot(oc, ray.direction);
	double c = dot(oc, oc) - sphere_radius * sphere_radius;
	double discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

// Linearly blends white and blue depending on
// the height of the y coordinate after scaling the ray direction to unit length.
Vec3 ray_color(const Ray& ray) {
	if(hit_sphere(Vec3(0, 0, -1), 0.5, ray)) {
		return Vec3(1, 0, 0);
	}

	Vec3 unit_direction = unit_vector(ray.direction); // scale to unit length
	double t = 0.5 * (unit_direction.y + 1.0); // Set scalar for lerp gradient

	// Standard lerp of the form L(t) = (1 - t) * start + t * end
	// First vec3 is the color white and the second is blue
	return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

void render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// Screen settings
	const int image_width = 256;
	const int image_height = 256;
	const int pixel_scalar = 4;

	// Camera settings
	const double viewport_height = 2.0;
	const double viewport_width = 2.0;
        const double focal_length = 1.0;
	
	//
	const Vec3 origin = Vec3(0, 0, 0);
	const Vec3 horizontal = Vec3(viewport_width, 0, 0);
	const Vec3 vertical = Vec3(0, viewport_height, 0);
	const Vec3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

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
			Vec3 pixel_color = ray_color(ray) * 255.0;
			SDL_SetRenderDrawColor(renderer, (int)pixel_color.x, (int)pixel_color.y, (int)pixel_color.z, 255);

			// Setup scaled rect and draw
			SDL_Rect rect;
			rect.x = x * pixel_scalar;
			rect.y = y * pixel_scalar;
			rect.w = pixel_scalar;
			rect.h = pixel_scalar;
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderPresent(renderer);
		}
	}
	SDL_RenderPresent(renderer);
}
