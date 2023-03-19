#pragma once

#include <SDL.h>

#include "game/entity/character.hpp"
#include "ui/inventory_slot.hpp"

class Player : public Character {
public:
	Player(Game& game, const Tilemap* tilemap, glm::vec2 position);

	void handle_input(const SDL_Event& event) override;
	void handle_per_frame_input(); // Will be called once per update. Looks at keyboard state rather than current event.
	void update() override;
	void render(f64 deltaTime, const Camera& camera) override;

	f64 get_walk_acceleration() { return 0.012; }
	f64 get_friction() { return 0.9; }
	f64 get_rotation_speed() { return 4.5; }

private:
	static constexpr const u32 NUM_INVENTORY_SLOTS = 4;
	InventorySlot inventory[NUM_INVENTORY_SLOTS];
	u32 inventorySelection = 0;
};
