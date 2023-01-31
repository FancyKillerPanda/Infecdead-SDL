#include "game/game.hpp"
#include "game/state/play_state.hpp"

PlayState::PlayState(Game& game) 
	: State(game), player(game, { 10, 10 }) {
}

void PlayState::handle_input(const SDL_Event& event) {
}

void PlayState::update() {
	animate();

	player.update();
}

void PlayState::render(f64 deltaTime) {
	for (Animation* animation : currentAnimations) {
		animation->render(game.get_renderer(), deltaTime);
	}

	player.render(deltaTime);
}
