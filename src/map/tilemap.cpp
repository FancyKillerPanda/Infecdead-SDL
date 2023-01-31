#include "map/tilemap.hpp"
#include "utility/log.hpp"

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
}
