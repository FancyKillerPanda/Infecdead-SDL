#include <chrono>

#include <glm/glm.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "common.hpp"
#include "animation/fade.hpp"
#include "game/game.hpp"
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

	mainMenuHomeButtons = ButtonGroup(renderer, {
		Text { renderer, primaryFont, "Play", SDL_Color { 0, 0, 0, 255 } },
		Text { renderer, primaryFont, "Help", SDL_Color { 0, 0, 0, 255 } },
		Text { renderer, primaryFont, "Options", SDL_Color { 0, 0, 0, 255 } },
		Text { renderer, primaryFont, "About", SDL_Color { 0, 0, 0, 255 } },
	}, {});
	mainMenuHomeButtons.set_render_function([](ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
		constexpr SDL_Color BASE_COLOUR = { 0, 0, 0, 255 };
		constexpr SDL_Color HOVER_COLOUR = { 255, 255, 0, 255 };
		constexpr SDL_Color PRESSED_COLOUR = { 192, 192, 0, 255 };
		
		SDL_Color colour = BASE_COLOUR;
		if (buttons.get_pressed_index() != -1) {
			if (buttons.get_pressed_index() == currentButton) {
				colour = PRESSED_COLOUR;
			}
		} else {
			if (buttons.get_hover_index() == currentButton) {
				colour = HOVER_COLOUR;
			}
		}
		
		glm::vec2 rectangleDimensions = { 240, 64 };
		glm::vec4 rectangleBox = { position - (rectangleDimensions / 2.0f), rectangleDimensions };
		s32 cornerRadius = 16;

		shapes::draw_rounded_rectangle(buttons.get_renderer(), rectangleBox, cornerRadius, 8, colour);
		buttons.texts[currentButton].change_colour(colour);
		buttons.texts[currentButton].render(position - (buttons.texts[currentButton].get_texture().dimensions / 2.0f));
	});

	currentState = GameState::MainMenu_Home;
	currentAnimations.push_back(new FadeAnimation(glm::vec4 { 0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, SDL_Color { 0, 0, 0, 255 }, SDL_Color { 0, 0, 0, 0 }, 100));
	
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

		switch (currentState) {
		case GameState::MainMenu_Home:
			mainMenuHomeButtons.handle_input(event);
			break;
		
		default:
			break;
		}
	}
}

void Game::update() {
	switch (currentState) {
	case GameState::None:
		log::error("Current game state is None. Quitting.");
		running = false;

		return;
	
	default:
		break;
	};

	for (u32 i = 0; i < currentAnimations.size(); ) {
		Animation* animation = currentAnimations[i];
		if (animation->update()) {
			currentAnimations.erase(currentAnimations.begin() + i);
		} else {
			i += 1;
		}
	}
}

void Game::render(f64 deltaTime) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	switch (currentState) {
	case GameState::MainMenu_Home:
		mainMenuHomeButtons.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT * 55 / 100 }, { 0, VIEWPORT_HEIGHT * 1 / 10 });
		break;
	
	default:
		break;
	};

	for (Animation* animation : currentAnimations) {
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
