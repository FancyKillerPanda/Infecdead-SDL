#include <algorithm>

#include "map/tilemap.hpp"
#include "utility/log.hpp"
#include "utility/utility.hpp"

Tilemap::Tilemap(const Tileset& tileset, const u8* filepath) 
	: tileset(tileset) {
	// TODO(fkp): Extract read entire file to separate method.
	// Reads the file data.
	FILE* file = fopen(filepath, "r");
	if (!file) {
		log::error("Failed to open tilemap filepath '%s'.", filepath);
		return;
	}

	fseek(file, 0, SEEK_END);
	u32 fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	u8* buffer = new u8[fileSize + 1];
	fread(buffer, fileSize, 1, file);
	buffer[fileSize] = 0;

	// JSON format parsing.
	json data = json::parse(buffer);
	delete[] buffer;

	mapDimensions = { data["width"], data["height"] };
	tileDimensions = { data["tilewidth"], data["tileheight"] };

	for (json layerData : data["layers"]) {
		TilemapLayer layer { layerData };
		if (layer.type == TilemapLayerType::TileLayer) {
			tileLayers.emplace_back(std::move(layer));
		} else {
			// TODO(fkp): Implement
		}
	}

	// Sorts the layers by z-index. Layers with the same z-index will be
	// rendered in the order that they are defined in the tilemap file.
	// A z-index of less than 0 will be rendered below entities.
	// A z-index equal to 0 is on the same level as entities, and will be rendered based on y-position.
	// A z-index of greater than 0 will be rendered above entities.
	std::stable_sort(tileLayers.begin(), tileLayers.end(), [](const TilemapLayer& left, const TilemapLayer& right) {
		return left.get_z_index() < right.get_z_index();
	});
}

void Tilemap::render_first_pass(const glm::vec2& playerPosition, f32 scale) {
	// Renders all the layers with z < 0.
	// TODO(fkp): These can be cached into a single texture.
	for (const TilemapLayer& layer : tileLayers) {
		if (layer.get_z_index() >= 0) {
			continue;
		}

		glm::vec2 currentCoordinates = { 0, 0 };
		const Texture& texture = tileset.get_texture();

		for (u8 tile : layer.get_data()) {
			// Data is 0 for transparent tiles.
			if (tile != 0) {
				SDL_Rect srcRect = tileset.get_tile_rect(tile - 1); // Offset by 1 to account for 0 being transparent.
				SDL_Rect destRect = to_rect(currentCoordinates * tileDimensions * scale, tileDimensions * scale);

				SDL_RenderCopy(texture.get_renderer(), texture.get(), &srcRect, &destRect);
			}

			currentCoordinates.x += 1;
			if (currentCoordinates.x == mapDimensions.x) {
				currentCoordinates.x = 0;
				currentCoordinates.y += 1;
			}
		}
	}

	// Renders all the layers with z == 0, where the tile y-position is
	// less than the player position.
	for (const TilemapLayer& layer : tileLayers) {
		if (layer.get_z_index() != 0) {
			continue;
		}

		// TODO(fkp): Render.
	}
}

void Tilemap::render_second_pass(const glm::vec2& playerPosition, f32 scale) {
	// Renders all the layers with z == 0, where the tile y-position is
	// greater than the player position.
	for (const TilemapLayer& layer : tileLayers) {
		if (layer.get_z_index() != 0) {
			continue;
		}

		// TODO(fkp): Render.
	}
	
	// Renders all the layers with z > 0.
	// TODO(fkp): These can be cached into a single texture.
	for (const TilemapLayer& layer : tileLayers) {
		if (layer.get_z_index() <= 0) {
			continue;
		}

		// TODO(fkp): Render.
	}
}