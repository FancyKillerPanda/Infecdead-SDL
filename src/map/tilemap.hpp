#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <SDL.h>

#include "common.hpp"
#include "map/layer.hpp"
#include "map/tileset.hpp"

class Tilemap {
public:
	Tilemap(const Tileset& tileset, const u8* filepath);
	Tilemap(const Tilemap&) = delete;
	Tilemap& operator=(const Tilemap&) = delete;

private:
	const Tileset& tileset;
	std::vector<TilemapLayer> tileLayers;

	glm::vec2 mapDimensions; // Number of columns and rows.
	glm::vec2 tileDimensions; // Internal size of each tile. Tilemap can be rendered at a different size (scaled).
};
