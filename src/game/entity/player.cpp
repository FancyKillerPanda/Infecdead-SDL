#include "game/game.hpp"
#include "game/entity/player.hpp"

Player::Player(Game& game, const Tilemap* tilemap, glm::vec2 position) 
	: Character(game, Type::Player, "res/characters/player.png", tilemap, position) {
}

void Player::handle_input() {
	f64 rotationRadians = glm::radians(rotation);
	glm::vec2 rotationVector = glm::normalize(glm::vec2 { cos(rotationRadians), -sin(rotationRadians) });
	const Uint8* keyboard = SDL_GetKeyboardState(nullptr);
	
	acceleration = { 0, 0 };
	if (keyboard[SDL_SCANCODE_W]) {
		acceleration = rotationVector * (f32) get_walk_acceleration();
	}
	if (keyboard[SDL_SCANCODE_S]) {
		acceleration = rotationVector * (f32) (-get_walk_acceleration() * 0.5);
	}
	if (keyboard[SDL_SCANCODE_A]) {
		rotation += get_rotation_speed();
	}
	if (keyboard[SDL_SCANCODE_D]) {
		rotation += 360.0;
		rotation -= get_rotation_speed();
	}

	rotation = fmod(rotation, 360.0);
}

void Player::update() {
	handle_input();

	velocity += acceleration;
	velocity *= get_friction();

	if (abs(velocity.x) < 0.0015) velocity.x = 0;
	if (abs(velocity.y) < 0.0015) velocity.y = 0;

	update_position();
	update_texture();
}

// void Player::render(f64 deltaTime) {
//
// }
