// CURRENTLY: at the equivelant of section 7. Finally fixed depth bug.
// Should be noted that we are still flipped on the y compared to tutorial.

#include <iostream>
#include <SDL2/SDL.h>
#include "tracer.h"
#include "world.h"
#include "random.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

int main() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error initializing SDL!" << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("SDL Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window) {
		std::cout << "Error creating window!" << std::endl;
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) {
		std::cout << "Error creating renderer!" << std::endl;
		return 1;
	}

    // Doesn't depend on order of instantiation, thank god. Referring to the depth order glitch
    World world;
    world.spheres.emplace_back(Sphere(Vec3(0, 0, -1), 0.5));
    world.spheres.emplace_back(Sphere(Vec3(0.5, 0, -2), 0.2));
    world.spheres.emplace_back(Sphere(Vec3(1.5, 0, -2), 0.2));
    world.spheres.emplace_back(Sphere(Vec3(2.5, 0, -2), 0.2));
    world.spheres.emplace_back(Sphere(Vec3(-0.5, 0, -2), 0.2));
    world.spheres.emplace_back(Sphere(Vec3(-1.5, 0, -2), 0.2));
    world.spheres.emplace_back(Sphere(Vec3(-2.5, 0, -2), 0.2));
    world.spheres.emplace_back(Sphere(Vec3(0, 100.5, -1), 100));

    // Debug movement vars
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    const double x_speed = 0.05;
    Vec3 cam_velocity;

	bool quit = false;
	while(!quit) {
		SDL_Event sdl_event;
		while(SDL_PollEvent(&sdl_event) != 0) {
			if(sdl_event.type == SDL_QUIT) {
				quit = true;
			}
            if(sdl_event.type == SDL_KEYDOWN) {
                switch(sdl_event.key.keysym.sym) {
                    case SDLK_LEFT :
                        left = true;
                        break;
                    case SDLK_RIGHT :
                        right = true;
                        break;
                    case SDLK_UP :
                        up = true;
                        break;
                    case SDLK_DOWN :
                        down = true;
                        break;
                    default :
                        break;
                }
            }
            if(sdl_event.type == SDL_KEYUP) {
                switch(sdl_event.key.keysym.sym) {
                    case SDLK_LEFT :
                        left = false;
                        break;
                    case SDLK_RIGHT :
                        right = false;
                        break;
                    case SDLK_UP :
                        up = false;
                        break;
                    case SDLK_DOWN :
                        down = false;
                        break;
                    default :
                        break;
                }
            }
	    }
        
        cam_velocity.x = 0;
        cam_velocity.z = 0;
        if(left) cam_velocity.x += x_speed;
        if(right) cam_velocity.x -= x_speed;
        if(up) cam_velocity.z += x_speed;
        if(down) cam_velocity.z -= x_speed;

        // Render (with a timer)
        Uint32 render_start_time = SDL_GetTicks();
        render(renderer, world, cam_velocity);
        Uint32 render_time = SDL_GetTicks() - render_start_time;
        std::cout << "Render took " << render_time << "ms" << std::endl;
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
