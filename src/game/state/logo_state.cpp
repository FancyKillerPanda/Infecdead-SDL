#include "animation/logo.hpp"
#include "animation/fade.hpp"
#include "game/game.hpp"
#include "game/state/logo_state.hpp"
#include "game/state/main_menu_state.hpp"

LogoState::LogoState(Game& game) 
	: State(game) {
	Animation* logoAnimation = new LogoAnimation();
	logoAnimation->renderAfterFinished = true;
	logoAnimation->spawnNext.push_back(new FadeAnimation(glm::vec4 { 0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT },
														 SDL_Color { 0, 0, 0, 0 }, SDL_Color { 0, 0, 0, 255 }, 120 ));

	currentAnimations.push_back(logoAnimation);
}

void LogoState::update() {
	animate();

	if (currentAnimations.back()->isFinished) {
		game.replace_state(new MainMenuState(game));
	}
}

void LogoState::render(f64 deltaTime) {
	SDL_SetRenderDrawColor(game.get_renderer(), 0, 0, 0, 255);
	SDL_RenderClear(game.get_renderer());

	for (Animation* animation : currentAnimations) {
		animation->render(game.get_renderer(), deltaTime);
	}
}
