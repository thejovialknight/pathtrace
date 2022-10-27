#include <thread>
#include <vector>
#include <iostream>

#include "cm_math.h"
#include "tracer.h"
#include "random.h"

void Pathtracer::render(const World& world) { 	
	// Render image
	for(int y = IMAGE_HEIGHT - 1; y >= 0; --y) {
		for(int x = 0; x < IMAGE_WIDTH; ++x) {
            render_pixel(world, *this, x, y);
        }
    }
}

void render_pixel(const World& world, Pathtracer& tracer, int x, int y) {
    // Get color. Once we get multiple samples we will
    // need to scale the color by the number of samples
    // i.e. scale = 1.0 / samples_per_pixel; c.r *- scale; etc...
    Vec3 pixel_color(0, 0, 0);
    for(int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
        double u = (x + random_double()) / (IMAGE_WIDTH - 1);
        double v = (y + random_double()) / (IMAGE_HEIGHT - 1);
        Ray ray = world.camera.get_ray(u, v);
        pixel_color = pixel_color + color_from_ray(ray, world, MAX_DEPTH) * 255.0;
    }
    double luminance_scalar = 1.0 / SAMPLES_PER_PIXEL;
    pixel_color.x = (luminance_scalar * pixel_color.x);
    pixel_color.y = (luminance_scalar * pixel_color.y);
    pixel_color.z = (luminance_scalar * pixel_color.z);
    tracer.pixel_colors[x + y * IMAGE_WIDTH] = pixel_color;
}

// Even more hacky approximation of a diffuse material. Probably not even be as performant.
// Here for learning purposes
Vec3 easy_reflection(HitInfo& hit_info) {
    Vec3 random_in_hemisphere = random_in_unit_sphere();
    if(dot(random_in_hemisphere, hit_info.normal) <= 0.0) {
        random_in_hemisphere = -random_in_hemisphere;
    }
    return (hit_info.point + random_in_hemisphere) - hit_info.point;
}

// Simpler approximation of lambertian reflection. Should be ever so slightly cheaper.
Vec3 lambertian_approximation(HitInfo& hit_info) {
    return (hit_info.point + hit_info.normal + random_in_unit_sphere()) - hit_info.point;
}

// Most accurate diffuse material reflection
Vec3 lambertian_reflection(HitInfo& hit_info) {
    return (hit_info.point + hit_info.normal + random_unit_vector()) - hit_info.point;
}

Vec3 color_from_ray(const Ray& ray, const World& world, int depth) {
    if(depth <= 0) {
        return Vec3(0, 0, 0);
    }

    double t_max = std::numeric_limits<double>::infinity();
    HitInfo hit_info(false); // Stores the closest hit

    // For different shapes just need more for loops, continuing to populate hit info based on proximity
    for(const Sphere& sphere : world.spheres) {
        if(hit_info.hit) {
            t_max = hit_info.t;
        }
        HitInfo temp_info = sphere.hit(ray, 0.001, t_max);
        if(temp_info.hit) {
            hit_info = temp_info;
        }
    }

    // If we hit a surface, shoot another ray from a random direction on a tangent unit sphere
    if(hit_info.hit) {
        bool reflecting = false;
        Vec3 bounce_direction;
        // If we are randomly scattering (diffuse case)
        if(random_double() >= hit_info.material->metallicity) {
            bounce_direction = hit_info.normal + random_unit_vector();
            if(bounce_direction.near_zero()) {
                bounce_direction = hit_info.normal;
            }
        }
        // If we are reflecting (metallic case)
        else {
            reflecting = true;
            Vec3 v = unit_vector(ray.direction);
            Vec3 n = hit_info.normal;
            bounce_direction = v - 2.0 * dot(v, n) * n;
        }
        
        Ray bounce_ray = Ray(hit_info.point, bounce_direction);
        Vec3 attenuation = hit_info.material->albedo;

        // If scattering light. 
        // The expression on the right side of the || only applies to reflected rays
        // To be honest, I'm not super sure why it's needed
        if(!reflecting || dot(bounce_ray.direction, hit_info.normal) > 0) {
            return attenuation * color_from_ray(bounce_ray, world, depth - 1);
        }

        // If absorbed light
        return Vec3(0, 0, 0);
        // return 0.5 * color_from_ray(Ray(hit_info.point, lambertian_reflection(hit_info)), world, current_depth - 1);
    }

    // If we didn't hit a surface, return the sky gradient
    Vec3 unit_direction = unit_vector(ray.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    // WHITE */ return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(1.0, 1.0, 1.0);
    // RED/BLUE */ return (1.0 - t) * Vec3(0.8, 0.2, 0.2) + t * Vec3(0.0, 0.0, 0.9);
    // BLACK/WHITE */ return (1.0 - t) * Vec3(0.9, 0.9, 0.9) + t * Vec3(0.1, 0.1, 0.1);
    // SLIGHT RED */ return (1.0 - t) * Vec3(0.9, 0.7, 0.7) + t * Vec3(0.2, 0.1, 0.1);
    /* SKY BLUE */ return (1.0 - t) * Vec3(0.5, 0.3, 0.9) + t * Vec3(0.9, 0.9, 0.9);
}

void Pathtracer::draw_frame(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
    for(int y = 0; y < IMAGE_HEIGHT; ++y) {
        for(int x = 0; x < IMAGE_WIDTH; ++x) {
            Vec3 pixel_color = pixel_colors[x + y * IMAGE_WIDTH];
            SDL_SetRenderDrawColor(renderer, (int)pixel_color.x, (int)pixel_color.y, (int)pixel_color.z, 255);
			SDL_Rect rect;
			rect.x = x * PIXEL_SCALAR;
			rect.y = y * PIXEL_SCALAR;
			rect.w = PIXEL_SCALAR;
			rect.h = PIXEL_SCALAR;
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}
