#pragma once

#include <SDL.h>

#include "game/entity/character.hpp"

class Player : public Character { 
public:
	Player(Game& game, glm::vec2 position);

	void handle_input(); // Will be called once per update. Looks at keyboard state rather than current event.
	void update() override;
	// void render(f64 deltaTime) override;

	f64 get_walk_acceleration() { return 0.32; }
	f64 get_friction() { return 0.009; }
	f64 get_rotation_speed() { return 7; }
};
