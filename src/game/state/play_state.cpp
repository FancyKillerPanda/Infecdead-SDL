#include "game/game.hpp"
#include "game/state/play_state.hpp"

PlayState::PlayState(Game& game) 
	: State(game), camera(worldMap, player), player(game, &worldMap, { 10, 10 }),
	  tileset(game.get_renderer(), "res/maps/tileset.json"), worldMap(tileset, "res/maps/world.json") {
}

void PlayState::handle_input(const SDL_Event& event) {
	player.handle_input(event);
}

void PlayState::update() {
	animate();

	player.update();
    camera.update();
}

void PlayState::render(f64 deltaTime) {
	for (Animation* animation : currentAnimations) {
		animation->render(game.get_renderer(), deltaTime);
	}

	worldMap.render_first_pass(worldMap.get_default_scale(), camera, player);
	player.render(deltaTime, camera);
	worldMap.render_second_pass(worldMap.get_default_scale(), camera, player);
}
