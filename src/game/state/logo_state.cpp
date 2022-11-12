#include "animation/logo.hpp"
#include "game/game.hpp"
#include "game/state/logo_state.hpp"

LogoState::LogoState(Game& game) 
	: State(game) {
	currentAnimations.push_back(new LogoAnimation());
}

void LogoState::update() {
	animate();
}

void LogoState::render(f64 deltaTime) {
	SDL_SetRenderDrawColor(game.get_renderer(), 0, 0, 0, 255);
	SDL_RenderClear(game.get_renderer());
}
