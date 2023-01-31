#include "game/game.hpp"
#include "game/state/play_state.hpp"
#include "map/tileset.hpp"

PlayState::PlayState(Game& game) 
	: State(game), player(game, { 10, 10 }) {
	Tileset tileset { game.get_renderer(), "res/maps/tileset.json" };

	SDL_Rect rect;
	rect = tileset.get_tile_rect(0);
	printf("Rect: (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
	rect = tileset.get_tile_rect(1);
	printf("Rect: (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
	rect = tileset.get_tile_rect(10);
	printf("Rect: (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
	rect = tileset.get_tile_rect(65);
	printf("Rect: (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
	rect = tileset.get_tile_rect(200);
	printf("Rect: (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
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
