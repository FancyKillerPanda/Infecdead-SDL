#pragma once

#include <SDL.h>

#include "common.hpp"

class Game {
public:
	Game();
	~Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void run();

private:
	bool create_window();

	void handle_input();
	void update();
	void render(f64 deltaTime);

private:
	bool running = false;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};
