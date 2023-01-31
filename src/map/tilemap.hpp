#pragma once

#include <vector>

#include <SDL.h>

#include "common.hpp"
#include "map/layer.hpp"
#include "map/tileset.hpp"

class Tilemap {
public:
	Tilemap(const Tileset& tileset, const u8* filepath);

private:
	const Tileset& tileset;
	std::vector<TilemapLayer> tileLayers;

	u32 width = 0;
	u32 height = 0;

	u32 tileWidth = 0;
	u32 tileHeight = 0;
};
