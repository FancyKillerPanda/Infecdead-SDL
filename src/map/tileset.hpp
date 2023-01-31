#pragma once

#include <SDL.h>

#include "common.hpp"
#include "graphics/texture.hpp"

class Tileset {
public:
	Tileset(SDL_Renderer* renderer, const u8* filepath);

	const Texture& get_texture() { return texture; }
	SDL_Rect get_tile_rect(u32 index);

private:
	Texture texture;

	u32 tileCount = 0;
	u32 numRows = 0;
	u32 numCols = 0;

	u32 tileWidth = 0;
	u32 tileHeight = 0;
};
