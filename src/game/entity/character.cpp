#include "game/game.hpp"
#include "game/entity/character.hpp"
#include "map/tilemap.hpp"

Character::Character(Game& game, Type type, const u8* walkSpritesheetPath, const Tilemap* tilemap, glm::vec2 position)
	: game(game), type(type), currentTilemap(tilemap), worldPosition(position) {
	walkSpritesheet = new Spritesheet(Texture { game.get_renderer(), walkSpritesheetPath }, { 16, 16 }, 32, 4);
	walkSpritesheet->animation = new SpritesheetAnimation(*walkSpritesheet, {}, 15);

	currentSpritesheet = walkSpritesheet;
}

void Character::update_position() {
	// TODO(fkp): Object collisions.
	worldPosition.x += velocity.x;
	worldPosition.y += velocity.y;

	// TODO(fkp): Clamping position.
}

void Character::update_texture() {
	currentSpritesheet->update();
	
	u32 row = (u32) (fmod(rotation + 22.5, 360.0) / 45.0);
	u32 column = currentSpritesheet->get_current_subrect_index() % currentSpritesheet->get_subrects_per_row();
	if (velocity == glm::vec2 { 0, 0 }) {
		column = 0;
	}

	currentSpritesheet->set_current_subrect_index((row * currentSpritesheet->get_subrects_per_row()) + column);
}

void Character::render(f64 deltaTime, const Camera& camera) {
	f32 scale = currentTilemap->get_default_scale();
	const glm::vec2& tileDimensions = currentTilemap->get_tileset().get_tile_dimensions();
	
	currentSpritesheet->render((worldPosition - camera.get_view_offset()) * tileDimensions * scale, dimensions * tileDimensions * scale);
}
