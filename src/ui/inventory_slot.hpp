#pragma once

#include "graphics/texture.hpp"

class Game;

class InventorySlot {
public:
	InventorySlot(Game& game);

	void render(glm::vec2 position, bool isSelected);

private:
	Texture baseTexture;
	Texture selectedTexture;
};
