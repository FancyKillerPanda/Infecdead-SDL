#include "game/game.hpp"
#include "game/state/main_menu_state.hpp"
#include "graphics/shapes.hpp"

MainMenuState::MainMenuState(Game& game)
	: State(game) {
	homeButtons = ButtonGroup(game.get_renderer(), {
		Text { game.get_renderer(), game.get_primary_font(), "Play", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Help", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Options", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "About", SDL_Color { 0, 0, 0, 255 } },
	}, {});
	homeButtons.set_render_function(button_render_function);
}

void MainMenuState::handle_input(const SDL_Event& event) {
	homeButtons.handle_input(event);
}

void MainMenuState::update() {

}

void MainMenuState::render(f64 deltaTime) {
	homeButtons.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT * 55 / 100 }, { 0, VIEWPORT_HEIGHT * 1 / 10 });
}

void MainMenuState::button_render_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
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
}
