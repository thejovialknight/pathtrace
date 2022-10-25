#include <iostream>
#include <SDL2/SDL.h>
#include "tracer.h"

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

	// Render test image
	render(renderer);

	bool quit = false;
	while(!quit) {
		SDL_Event sdl_event;
		while(SDL_PollEvent(&sdl_event) != 0) {
			if(sdl_event.type == SDL_QUIT) {
				quit = true;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
