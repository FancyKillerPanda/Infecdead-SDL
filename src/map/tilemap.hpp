#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <SDL.h>

#include "common.hpp"
#include "game/camera.hpp"
#include "map/layer.hpp"
#include "map/tileset.hpp"

class Tilemap {
public:
	Tilemap(const Tileset& tileset, const u8* filepath);
	Tilemap(const Tilemap&) = delete;
	Tilemap& operator=(const Tilemap&) = delete;

	void render_first_pass(f32 scale, const Camera& camera); // Renders everything below the player.
	void render_second_pass(f32 scale); // Renders everything above the player.

    const Tileset& get_tileset() const { return tileset; }
    const glm::vec2& get_map_dimensions() const { return mapDimensions; }
    const f32 get_default_scale() const { return 2.0; } // TODO(fkp): Read this from the map data.

private:
	const Tileset& tileset;
	std::vector<TilemapLayer> tileLayers;

	glm::vec2 mapDimensions; // Number of columns and rows.

	Texture firstPassTexture;
	Texture secondPassTexture;
};
