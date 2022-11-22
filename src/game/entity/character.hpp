#pragma once

#include <glm/glm.hpp>

#include "common.hpp"
#include "graphics/spritesheet.hpp"

class Game;

class Character {
public:
	enum class Type {
		Player,
	};

public:
	Character(Game& game, Type type, const u8* walkSpritesheetPath, glm::vec2 position);
	virtual ~Character() {}

	virtual void update() = 0;
	virtual void render(f64 deltaTime);

protected:
	void update_position();
	void update_texture();

protected:
	Game& game;
	Type type;

	glm::vec2 worldPosition {};
	f64 rotation = 0.0;

	glm::vec2 velocity {};
	glm::vec2 acceleration {};

	Spritesheet* currentSpritesheet = nullptr;
	Spritesheet* walkSpritesheet = nullptr;

	f64 health = 1.0;
};
