#include <chrono>

#include <glm/glm.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "common.hpp"
#include "animation/fade.hpp"
#include "game/game.hpp"
#include "game/state/logo_state.hpp"
#include "game/state/main_menu_state.hpp"
#include "graphics/shapes.hpp"
#include "graphics/text.hpp"
#include "graphics/texture.hpp"
#include "utility/log.hpp"
#include "utility/utility.hpp"

// TODO(fkp): Allow toggling between fullscreen and windowed.
bool FULLSCREEN = false;

// Number of milliseconds for each game tick.
f64 MS_PER_UPDATE = 10.0;

Game::Game() {
	create_window();

	constexpr const u8* FONT_PATH = "res/fonts/Pixeltype.ttf";
	constexpr const u32 FONT_SIZE = 48;
	primaryFont = TTF_OpenFont(FONT_PATH, FONT_SIZE);
	if (!primaryFont) {
		log::error("Failed to open font '%s' at size %u.\n%s", FONT_PATH, FONT_SIZE, SDL_GetError());
		return;
	}

	// push_state(new LogoState(*this));
	push_state(new MainMenuState(*this));
	
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

		stateStack.back()->handle_input(event);
	}
}

void Game::update() {
	stateStack.back()->update();
}

void Game::render(f64 deltaTime) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	stateStack.back()->render(deltaTime);

	// TODO(fkp): Should this be done in the state?
	for (Animation* animation : stateStack.back()->currentAnimations) {
		animation->render(renderer, deltaTime);
	}

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (!renderer) {
		log::error("SDL_CreateRenderer() failed.\n", SDL_GetError());
		return false;
	}

	log::info("Created SDL renderer.");

	// Sets attributes for rendering.
	SDL_RenderSetLogicalSize(renderer, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return true;
}

void Game::push_state(State* state) {
	stateStack.push_back(state);
}

void Game::pop_state() {
	if (stateStack.empty()) {
		log::error("Attempting to pop an empty state stack.");
		return;
	}

	delete stateStack.back();
	stateStack.pop_back();
}

void Game::replace_state(State* state) {
	pop_state();
	push_state(state);
}
