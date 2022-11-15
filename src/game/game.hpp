#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "common.hpp"
#include "animation/animation.hpp"
#include "game/state/state.hpp"

enum class GameState {
	None,
	MainMenu_Home,
};

class Game {
public:
	Game();
	~Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void run();

	void push_state(State* state);
	void pop_state();
	void replace_state(State* state);

	SDL_Renderer* get_renderer() { return renderer; }
	TTF_Font* get_title_font() { return titleFont; }
	TTF_Font* get_primary_font() { return primaryFont; }

private:
	bool create_window();

	void handle_input();
	void update();
	void render(f64 deltaTime);

private:
	bool running = false;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	std::vector<State*> stateStack;

	TTF_Font* titleFont = nullptr;
	TTF_Font* primaryFont = nullptr;
};
