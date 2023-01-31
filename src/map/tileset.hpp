#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

#include "common.hpp"
#include "graphics/texture.hpp"

class Tileset {
public:
	Tileset(SDL_Renderer* renderer, const u8* filepath);
	Tileset(const Tileset&) = delete;
	Tileset& operator=(const Tileset&) = delete;

	const Texture& get_texture() const { return texture; }
	SDL_Rect get_tile_rect(u32 index) const;

private:
	Texture texture;

	u32 tileCount = 0;
	glm::vec2 dimensions; // Number of columns and rows.
	glm::vec2 tileDimensions; // Width and height of each tile.
};
