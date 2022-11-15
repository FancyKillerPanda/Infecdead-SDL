#pragma once

#include "game/state/state.hpp"

class PlayState : public State {
public:
	PlayState(Game& game);

	void handle_input(const SDL_Event& event) override;
	void update() override;
	void render(f64 deltaTime) override;
};
