#pragma once

#include <glm/glm.hpp>

#include "game/state/state.hpp"
#include "ui/button_group.hpp"

class MainMenuState : public State {
	enum class Page {
		Home,
		Help,
		Options,
		About,
	};
	
public:
	MainMenuState(Game& game);

	void handle_input(const SDL_Event& event) override;
	void update() override;
	void render(f64 deltaTime) override;

private:
	static void button_render_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);
	static glm::vec2 button_get_dimensions_function(ButtonGroup& buttons, u32 currentButton);

private:
	std::vector<Page> pageStack;

	ButtonGroup backButton;
	ButtonGroup homeButtons;
	Text helpText;
	Text aboutText;
};
