#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "game/state/state.hpp"
#include "ui/button_group.hpp"

class MainMenuState : public State {
	enum class Page {
		Home,
		ProfileSelection,
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
	static void render_text_button(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);
	static std::vector<glm::vec4> get_text_button_hitboxes(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);

	static void render_profile_button(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);
	static std::vector<glm::vec4> get_profile_button_hitboxes(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);

private:
	std::vector<Page> pageStack;

	ButtonGroup backButton;
	ButtonGroup homeButtons;
	ButtonGroup profileSelectionButtons;

	Text titleText;
	Text helpText;
	Text aboutText;
};
