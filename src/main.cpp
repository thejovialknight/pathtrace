#include <iostream>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 256

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
	}

	// Render test image
	SDL_RenderClear(renderer);
	for(int y = WINDOW_HEIGHT - 1; y >= 0; --y) {
		for(int x = 0; x < WINDOW_WIDTH; ++x) {
			double r = double(y) / (WINDOW_WIDTH - 1);
			double g = double(x) / (WINDOW_HEIGHT - 1);
			double b = 0.25;
			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);
			SDL_SetRenderDrawColor(renderer, ir, ig, ib, 255);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	SDL_RenderPresent(renderer);

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
