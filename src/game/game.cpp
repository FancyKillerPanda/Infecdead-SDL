#include <chrono>

#include <glm/glm.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "common.hpp"
#include "game/game.hpp"
#include "utility/log.hpp"
#include "utility/utility.hpp"

// TODO(fkp): Allow toggling between fullscreen and windowed.
bool FULLSCREEN = false;

// Number of milliseconds for each game tick.
f64 MS_PER_UPDATE = 10.0;

Game::Game() {
	create_window();

	running = true;
	log::info("Game is running.");
}

Game::~Game() {
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	log::info("Destroyed SDL renderer.");

	SDL_DestroyWindow(window);
	window = nullptr;
	log::info("Destroyed SDL window.");
}

void Game::run() {
	std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
	f64 lagMs = 0.0;
	
	TTF_Font* font = TTF_OpenFont("res/fonts/Pixeltype.ttf", 30);
	if (!font) {
		log::error("Failed to open font.\n%s", SDL_GetError());
		return;
	}

	SDL_Surface* textSurface = TTF_RenderUTF8_Solid_Wrapped(font, "This is some test text.", SDL_Color { 0, 0, 0, 255 }, 0);
	if (!textSurface) {
		log::error("Failed to create text surface.\n%s", SDL_GetError());
		return;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textSurface) {
		log::error("Failed to create text texture.\n%s", SDL_GetError());
		return;
	}
	
	while (running) {
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		f64 elapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count() / 1000.0;
		previousTime = currentTime;
		lagMs += elapsedMs;

		handle_input();

		while (lagMs >= MS_PER_UPDATE) {
			update();
			lagMs -= MS_PER_UPDATE;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, textTexture, nullptr, nullptr);
		render(lagMs / MS_PER_UPDATE);
	}
}

void Game::handle_input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			log::info("Quitting game.");

			break;
		}
	}
}

void Game::update() {
}

void Game::render(f64 deltaTime) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

bool Game::create_window() {
	// Determines an appropriate window size.
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	glm::vec2 windowDimensions;
	SDL_WindowFlags windowFlags;

	if (FULLSCREEN) {
		windowDimensions = { displayMode.w, displayMode.h };
		windowFlags = SDL_WINDOW_FULLSCREEN;
	} else {
		windowFlags = SDL_WINDOW_SHOWN;
		
		u32 size = min(displayMode.w / 16, displayMode.h / 9);
		size -= 10; // Makes the window a bit smaller than the screen size.
		windowDimensions = { size * 16.0f, size * 9.0f };

		if (windowDimensions.x > 1920 && windowDimensions.y > 1080) {
			windowDimensions = { 1920, 1080 };
		}
	}

	// Creates the window and renderer objects.
	window = SDL_CreateWindow("Infecdead", 0, 0, windowDimensions.x, windowDimensions.y, windowFlags);
	if (!window) {
		log::error("SDL_CreateWindow() failed.\n", SDL_GetError());
		return false;
	}

	log::info("Created SDL window.");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		log::error("SDL_CreateRenderer() failed.\n", SDL_GetError());
		return false;
	}

	log::info("Created SDL renderer.");

	// Sets attributes for rendering.
	SDL_RenderSetLogicalSize(renderer, 1920, 1080);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return true;
}
