#pragma once

#include <SDL.h>

#include "common.hpp"
#include "ui/button_group.hpp"

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

private:
	bool create_window();

	void handle_input();
	void update();
	void render(f64 deltaTime);

private:
	static constexpr const u32 VIEWPORT_WIDTH = 1920;
	static constexpr u32 VIEWPORT_HEIGHT = 1080;

	bool running = false;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	GameState previousState = GameState::None;
	GameState currentState = GameState::None;
	GameState nextState = GameState::None;

	TTF_Font* primaryFont = nullptr;

	ButtonGroup mainMenuHomeButtons;
};
