#include <algorithm>

#include "SDL_render.h"
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

	// Creates textures for layers which can be rendered whole.
	SDL_Renderer* renderer = tileset.get_texture().get_renderer();
	glm::vec2 textureDimensions = mapDimensions * tileDimensions;
	
	SDL_Texture* firstTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
												  (s32) textureDimensions.x, (s32) textureDimensions.y);
	if (!firstTexture) {
		log::error("Failed to create first tilemap renderable texture.\n%s", SDL_GetError());
	}

	SDL_Texture* secondTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
												   (s32) textureDimensions.x, (s32) textureDimensions.y);
	if (!secondTexture) {
		log::error("Failed to create second tilemap renderable texture.\n%s", SDL_GetError());
	}

	SDL_SetTextureBlendMode(firstTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(secondTexture, SDL_BLENDMODE_BLEND);
	
	firstPassTexture = Texture { renderer, firstTexture };
	secondPassTexture = Texture { renderer, secondTexture };

	// Renders applicable layers to the textures.
	for (const TilemapLayer& layer : tileLayers) {
		if (layer.get_z_index() <= 0) {
			layer.render_to_texture(firstPassTexture, tileset, tileDimensions);
		} else if (layer.get_z_index() > 0) {
			layer.render_to_texture(secondPassTexture, tileset, tileDimensions);
		}
	}
}

void Tilemap::render_first_pass(f32 scale) {
	// Renders all the layers with z <= 0.
	SDL_Rect dstRect = to_rect({ 0, 0 }, firstPassTexture.dimensions * scale);
	SDL_RenderCopy(firstPassTexture.get_renderer(), firstPassTexture.get(), nullptr, &dstRect);
}

void Tilemap::render_second_pass(f32 scale) {
	// Renders all the layers with z > 0.
	SDL_Rect dstRect = to_rect({ 0, 0 }, secondPassTexture.dimensions * scale);
	SDL_RenderCopy(secondPassTexture.get_renderer(), secondPassTexture.get(), nullptr, &dstRect);
}
