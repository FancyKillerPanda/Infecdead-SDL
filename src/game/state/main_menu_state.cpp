#include "animation/fade.hpp"
#include "game/game.hpp"
#include "game/state/main_menu_state.hpp"
#include "graphics/shapes.hpp"

constexpr glm::vec2 MENU_BUTTON_RECTANGLE_DIMENSIONS = { 240, 64 };
constexpr SDL_Color MENU_BASE_COLOUR = { 0, 0, 0, 255 };
constexpr SDL_Color MENU_HOVER_COLOUR = { 255, 255, 0, 255 };
constexpr SDL_Color MENU_PRESSED_COLOUR = { 192, 192, 0, 255 };

constexpr const u8* HELP_TEXT =
R"(Welcome to the town of Infecdead (a fairly apt name for the current situation)!

This town has been overrun by zombies, and it's now up to you to save your people.
Why you, specifically? Because you're the guy the WASD keys control, duh!

Navigate the map and collect items from conveniently placed chests. How nice!
But beware, you can only carry four items before the weight of the situation
you are in brings you crashing down. So what will it be? Weapons? Defence? Med-kits?)";

constexpr const u8* ABOUT_TEXT =
R"(Infecdead v0.0.1

Created by FancyKillerPanda
https://github.com/FancyKillerPanda/
https://fancykillerpanda.itch.io/


More from me:
https://fancykillerpanda.itch.io/jumper/


Texture assets were inspired by a variety of sources in the public
domain, notably Kenney's work on https://opengameart.org/)";

MainMenuState::MainMenuState(Game& game)
	: State(game) {
	homeButtons = ButtonGroup(game.get_renderer(), {
		Text { game.get_renderer(), game.get_primary_font(), "Play", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Help", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Options", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "About", SDL_Color { 0, 0, 0, 255 } },
	}, {});
	homeButtons.set_render_function(render_text_button);
	homeButtons.set_get_dimensions_function(get_text_button_dimensions);

	backButton = ButtonGroup(game.get_renderer(), {
		Text { game.get_renderer(), game.get_primary_font(), "<-- Back", SDL_Color { 0, 0, 0, 255 } },
	}, {});
	backButton.set_render_function(render_text_button);
	backButton.set_get_dimensions_function(get_text_button_dimensions);

	helpText = Text { game.get_renderer(), game.get_primary_font(), HELP_TEXT, SDL_Color { 0, 0, 0, 255 } };
	aboutText = Text { game.get_renderer(), game.get_primary_font(), ABOUT_TEXT, SDL_Color { 0, 0, 0, 255 } };

	currentAnimations.push_back(new FadeAnimation(glm::vec4 { 0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, SDL_Color { 0, 0, 0, 255 }, SDL_Color { 0, 0, 0, 0 }, 100));
	pageStack.push_back(Page::Home);
}

void MainMenuState::handle_input(const SDL_Event& event) {
	switch (pageStack.back()) {
	case Page::Home: {
		s32 result = homeButtons.handle_input(event);
		switch (result) {
		case 0: // Play
			break;

		case 1: // Help
			pageStack.push_back(Page::Help);
			homeButtons.reset();
			break;

		case 2: // Options
			pageStack.push_back(Page::Options);
			homeButtons.reset();
			break;

		case 3: // About
			pageStack.push_back(Page::About);
			homeButtons.reset();
			break;
		
		default:
			break;
		}

		break;
	}

	default:
		break;
	}

	if (pageStack.back() != Page::Home) {
		if (backButton.handle_input(event) != -1) {
			backButton.reset();
			pageStack.pop_back();
		}
	}
}

void MainMenuState::update() {
	animate();
}

void MainMenuState::render(f64 deltaTime) {
	switch (pageStack.back()) {
	case Page::Home:
		homeButtons.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT * 55 / 100 }, { 0, VIEWPORT_HEIGHT * 1 / 10 });
		break;

	case Page::Help:
		helpText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2 });
		break;
	
	case Page::About:
		aboutText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2 });
		break;
	
	default:
		break;
	}

	if (pageStack.back() != Page::Home) {
		backButton.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT * 4 / 5 }, { 0, 0 });
	}
}

void MainMenuState::render_text_button(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
	SDL_Color colour = buttons.get_current_colour(currentButton, MENU_BASE_COLOUR, MENU_HOVER_COLOUR, MENU_PRESSED_COLOUR);
	
	glm::vec2 rectangleDimensions = MENU_BUTTON_RECTANGLE_DIMENSIONS;
	glm::vec4 rectangleBox = { position - (rectangleDimensions / 2.0f), rectangleDimensions };
	s32 cornerRadius = 16;

	shapes::draw_rounded_rectangle(buttons.get_renderer(), rectangleBox, cornerRadius, 8, colour);
	buttons.texts[currentButton].change_colour(colour);
	buttons.texts[currentButton].render(position);
}

glm::vec2 MainMenuState::get_text_button_dimensions(ButtonGroup& buttons, u32 currentButton) {
	return MENU_BUTTON_RECTANGLE_DIMENSIONS;
}