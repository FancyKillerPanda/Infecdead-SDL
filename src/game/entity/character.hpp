#pragma once

#include <glm/glm.hpp>

#include "common.hpp"
#include "graphics/spritesheet.hpp"

class Game;
class Tilemap;
class Camera;

class Character {
public:
	enum class Type {
		Player,
	};

public:
	Character(Game& game, Type type, const u8* walkSpritesheetPath, const Tilemap* tilemap, glm::vec2 position);
	virtual ~Character() {}

	virtual void handle_input(const SDL_Event& event) {};
	virtual void update() = 0;
	virtual void render(f64 deltaTime, const Camera& camera);

	const glm::vec2& get_world_position() const { return worldPosition; }
	const glm::vec2& get_dimensions() const { return dimensions; }

protected:
	void update_position();
	void update_texture();

protected:
	Game& game;
	Type type;

    const Tilemap* currentTilemap = nullptr;
	glm::vec2 worldPosition {}; // World is in the range [0, number of tiles) in each direction.
	f64 rotation = 0.0;
	glm::vec2 dimensions = { 2, 2 }; // Refers to the output size in world space units.

	glm::vec2 velocity {};
	glm::vec2 acceleration {};

	Spritesheet* currentSpritesheet = nullptr;
	Spritesheet* walkSpritesheet = nullptr;

	f64 health = 1.0;
};
