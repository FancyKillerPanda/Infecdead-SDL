#include <stdio.h>

#include <nlohmann/json.hpp>

#include "map/tileset.hpp"
#include "utility/log.hpp"

using json = nlohmann::json;

Tileset::Tileset(SDL_Renderer* renderer, const u8* filepath) {
	// TODO(fkp): Extract read entire file to separate method.
	// Reads the file data.
	FILE* file = fopen(filepath, "r");
	if (!file) {
		log::error("Failed to open tileset filepath '%s'.", filepath);
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

	// TODO(fkp): Hardcoding the path here seems bad.
	// TODO(fkp): We need the string to last for logging the filepath when the texture
	//            is destroyed, but this doesn't seem like a great way to do it.
	std::string* imageFilepath = new std::string("res/maps/");
	*imageFilepath += std::string(data["image"]);

	texture = Texture { renderer, imageFilepath->c_str() };
	if (texture.dimensions.x != data["imagewidth"] || texture.dimensions.y != data["imageheight"]) {
		log::warn("Tileset texture dimensions do not match dimensions specified in file.");
	}

	tileCount = data["tilecount"];
	dimensions = glm::vec2 { data["columns"], tileCount / (u32) data["columns"] };
	tileDimensions = glm::vec2 { data["tilewidth"], data["tileheight"] };

	if (data["margin"] != 0 || data["spacing"] != 0) {
		log::warn("Tileset currently only supports a margin and spacing of 0.");
	}
}

SDL_Rect Tileset::get_tile_rect(u32 index) const {
	if (index >= tileCount) {
		log::warn("Attempting to get invalid tileset tile rect.");
		return { 0, 0, 0, 0 };
	}

	u32 row = index / ((u32) dimensions.x);
	u32 col = index % ((u32) dimensions.x);

	return { (s32) (col * tileDimensions.x),
			 (s32) (row * tileDimensions.y),
			 (s32) tileDimensions.x,
			 (s32) tileDimensions.y };
}
