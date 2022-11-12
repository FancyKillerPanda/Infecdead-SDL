#pragma once

#include <SDL.h>

#include "common.hpp"

class Game;

class State {
public:
	State(Game& game) : game(game) {};

	virtual void handle_input(const SDL_Event& event) {}
	virtual void update() {}
	virtual void render(f64 deltaTime) {}

private:
	Game& game;
};
