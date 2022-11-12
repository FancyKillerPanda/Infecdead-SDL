#pragma once

#include "game/state/state.hpp"
#include "ui/button_group.hpp"

class MainMenuState : public State {
public:
	MainMenuState(Game& game);

	void handle_input(const SDL_Event& event) override;
	void update() override;
	void render(f64 deltaTime) override;

private:
	static void button_render_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);

private:
	ButtonGroup homeButtons;
};