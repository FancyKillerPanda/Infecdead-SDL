#include "game/game.hpp"
#include "game/state/play_state.hpp"

PlayState::PlayState(Game& game) 
	: State(game) {

}

void PlayState::handle_input(const SDL_Event& event) {

}

void PlayState::update() {
	animate();
}

void PlayState::render(f64 deltaTime) {
	for (Animation* animation : currentAnimations) {
		animation->render(game.get_renderer(), deltaTime);
	}
}
