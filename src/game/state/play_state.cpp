#include "game/game.hpp"
#include "game/state/play_state.hpp"

PlayState::PlayState(Game& game) 
	: State(game), player(game, { 10, 10 }),
	  tileset(game.get_renderer(), "res/maps/tileset.json"), worldMap(tileset, "res/maps/world.json") {
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

	worldMap.render_first_pass(2.0);
	player.render(deltaTime);
	worldMap.render_second_pass(2.0);
}
