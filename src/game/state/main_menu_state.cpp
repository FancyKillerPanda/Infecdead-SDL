#include "animation/fade.hpp"
#include "game/game.hpp"
#include "game/state/main_menu_state.hpp"
#include "game/state/play_state.hpp"
#include "graphics/shapes.hpp"

constexpr glm::vec2 MENU_BUTTON_RECTANGLE_DIMENSIONS = { 240, 64 };
constexpr glm::vec2 PROFILE_BUTTON_IMAGE_SIZE = { 256, 256 };

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
	// Home buttons
	homeButtons = ButtonGroup(game.get_renderer(), {
		Text { game.get_renderer(), game.get_primary_font(), "Play", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Help", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Options", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "About", SDL_Color { 0, 0, 0, 255 } },
	}, {});
	homeButtons.set_render_function(render_text_button);
	homeButtons.set_get_hitboxes_function(get_text_button_hitboxes);

	// Profile selection buttons
	profileSelectionButtons = ButtonGroup(game.get_renderer(), {
		Text { game.get_renderer(), game.get_primary_font(), "Profile 1", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Profile 2", SDL_Color { 0, 0, 0, 255 } },
		Text { game.get_renderer(), game.get_primary_font(), "Profile 3", SDL_Color { 0, 0, 0, 255 } },
	}, {});
	profileSelectionButtons.set_render_function(render_profile_button);
	profileSelectionButtons.set_get_hitboxes_function(get_profile_button_hitboxes);

	// Back button
	backButton = ButtonGroup(game.get_renderer(), {
		Text { game.get_renderer(), game.get_primary_font(), "<-- Back", SDL_Color { 0, 0, 0, 255 } },
	}, {});
	backButton.set_render_function(render_text_button);
	backButton.set_get_hitboxes_function(get_text_button_hitboxes);

	// Text
	titleText = Text { game.get_renderer(), game.get_title_font(), "INFECDEAD", SDL_Color { 0, 0, 0, 255 } };
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
			pageStack.push_back(Page::ProfileSelection);
			homeButtons.reset();
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

	case Page::ProfileSelection: {
		s32 result = profileSelectionButtons.handle_input(event);
		switch (result) {
		case 0:
		case 1:
		case 2:
			game.replace_state(new PlayState(game));
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
		titleText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 4 });
		homeButtons.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT * 55 / 100 }, { 0, VIEWPORT_HEIGHT * 1 / 10 });
		break;

	case Page::ProfileSelection:
		titleText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 4 });
		profileSelectionButtons.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT * 50 / 100 }, { VIEWPORT_WIDTH * 2 / 10, 0 });
		break;

	case Page::Help:
		titleText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 4 });
		helpText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2 });
		break;
	
	case Page::Options:
		titleText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 4 });
		break;
	
	case Page::About:
		titleText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 4 });
		aboutText.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2 });
		break;
	
	default:
		break;
	}

	if (pageStack.back() != Page::Home) {
		backButton.render({ VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT * 4 / 5 }, { 0, 0 });
	}

	for (Animation* animation : currentAnimations) {
		animation->render(game.get_renderer(), deltaTime);
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

std::vector<glm::vec4> MainMenuState::get_text_button_hitboxes(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
	return { glm::vec4 { position - (MENU_BUTTON_RECTANGLE_DIMENSIONS / 2.0f), MENU_BUTTON_RECTANGLE_DIMENSIONS } };
}

void MainMenuState::render_profile_button(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
	SDL_Color colour = buttons.get_current_colour(currentButton, MENU_BASE_COLOUR, MENU_HOVER_COLOUR, MENU_PRESSED_COLOUR);

	glm::vec4 rectangleBox = { position - (PROFILE_BUTTON_IMAGE_SIZE / 2.0f), PROFILE_BUTTON_IMAGE_SIZE };
	shapes::draw_rounded_rectangle(buttons.get_renderer(), rectangleBox, 16, 8, colour);

	render_text_button(buttons, currentButton, { position.x, rectangleBox.y + rectangleBox.w + MENU_BUTTON_RECTANGLE_DIMENSIONS.y });
}

std::vector<glm::vec4> MainMenuState::get_profile_button_hitboxes(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
	glm::vec4 rectangleBox = { position - (PROFILE_BUTTON_IMAGE_SIZE / 2.0f), PROFILE_BUTTON_IMAGE_SIZE };
	std::vector<glm::vec4> hitboxes = get_text_button_hitboxes(buttons, currentButton, { position.x, rectangleBox.y + rectangleBox.w + MENU_BUTTON_RECTANGLE_DIMENSIONS.y });
	hitboxes.emplace_back(rectangleBox);
	
	return hitboxes;
}
