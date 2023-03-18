#pragma once

#include <glm/glm.hpp>

#include "common.hpp"
#include "graphics/spritesheet.hpp"

class Game;
class Tilemap;

class Character {
public:
	enum class Type {
		Player,
	};

public:
	Character(Game& game, Type type, const u8* walkSpritesheetPath, const Tilemap* tilemap, glm::vec2 position);
	virtual ~Character() {}

	virtual void update() = 0;
	virtual void render(f64 deltaTime);

	const glm::vec2& get_world_position() const { return worldPosition; }

protected:
	void update_position();
	void update_texture();

protected:
	Game& game;
	Type type;

    const Tilemap* currentTilemap = nullptr;
	glm::vec2 worldPosition {}; // World is in the range [0, number of tiles) in each direction.
	f64 rotation = 0.0;

	glm::vec2 velocity {};
	glm::vec2 acceleration {};

	Spritesheet* currentSpritesheet = nullptr;
	Spritesheet* walkSpritesheet = nullptr;

	f64 health = 1.0;
};
