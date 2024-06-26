#include <algorithm>

#include "game/entity/player.hpp"
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
	glm::vec2 textureDimensions = mapDimensions * tileset.get_tile_dimensions();
	
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
		if (layer.get_z_index() < 0) {
			layer.render_to_texture(firstPassTexture, tileset, tileset.get_tile_dimensions());
		} else if (layer.get_z_index() > 0) {
			layer.render_to_texture(secondPassTexture, tileset, tileset.get_tile_dimensions());
		}
	}
}

// TODO(fkp): Code similarity/duplication with second pass.
void Tilemap::render_first_pass(f32 scale, const Camera& camera, const Player& player) {
	render_internal(scale, camera, firstPassTexture);

	for (const TilemapLayer& layer : tileLayers) {
		if (layer.get_z_index() != 0) {
			continue;
		}
		
		const std::vector<u8>& layerData = layer.get_data();
		if (layerData.size() == 0) {
			continue; // TODO(fkp): Figure out why there's a layer with no data.
		}

		glm::vec2 playerBottom = player.get_world_position();
		playerBottom.y += player.get_dimensions().y / 2.0f;

		glm::vec2 worldSpaceViewport = camera.get_world_space_viewport();
		glm::vec2 cameraViewOffset = camera.get_view_offset();
		glm::vec2 viewOffset = { (u32) cameraViewOffset.x, (u32) cameraViewOffset.y };
	
		const glm::vec2& layerDimensions = layer.get_dimensions();
		const glm::vec2& tileSize = tileset.get_tile_dimensions() * scale;

		SDL_Renderer* renderer = tileset.get_texture().get_renderer();
		SDL_Texture* tilesetTexture = tileset.get_texture().get();

		for (u32 viewRow = 0; viewRow < worldSpaceViewport.y; viewRow++) {
			for (u32 viewCol = 0; viewCol < worldSpaceViewport.x; viewCol++) {
				glm::vec2 position = glm::vec2 { viewCol, viewRow } + viewOffset;

				if (position.y >= (u32) playerBottom.y) {
					goto endLoop;
				}
				
				u32 index = ((u32) position.y * layerDimensions.x) + (u32) position.x;
				u8 tile = layerData[index];

				if (tile != 0) {
					SDL_Rect srcRect = tileset.get_tile_rect(tile - 1); // Offset by 1 to account for 0 being transparent.
					SDL_Rect destRect = to_rect((position - cameraViewOffset) * tileSize, tileSize);

					SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
				}
			}
		}

	endLoop:
		;
	}
}

// TODO(fkp): Code similarity/duplication with first pass.
void Tilemap::render_second_pass(f32 scale, const Camera& camera, const Player& player) {
	for (const TilemapLayer& layer : tileLayers) {
		if (layer.get_z_index() != 0) {
			continue;
		}
		
		const std::vector<u8>& layerData = layer.get_data();
		if (layerData.size() == 0) {
			continue; // TODO(fkp): Figure out why there's a layer with no data.
		}

		glm::vec2 playerBottom = player.get_world_position();
		playerBottom.y += player.get_dimensions().y / 2.0f;

		glm::vec2 worldSpaceViewport = camera.get_world_space_viewport();
		glm::vec2 cameraViewOffset = camera.get_view_offset();
		glm::vec2 viewOffset = { (u32) cameraViewOffset.x, (u32) cameraViewOffset.y };

		const glm::vec2& layerDimensions = layer.get_dimensions();
		const glm::vec2& tileSize = tileset.get_tile_dimensions() * scale;

		SDL_Renderer* renderer = tileset.get_texture().get_renderer();
		SDL_Texture* tilesetTexture = tileset.get_texture().get();

		for (u32 viewRow = 0; viewRow < worldSpaceViewport.y; viewRow++) {
			for (u32 viewCol = 0; viewCol < worldSpaceViewport.x; viewCol++) {
				glm::vec2 position = glm::vec2 { viewCol, viewRow } + viewOffset;

				if (position.y < (u32) playerBottom.y) {
					goto nextRow;
				}
				
				u32 index = ((u32) position.y * layerDimensions.x) + (u32) position.x;
				u8 tile = layerData[index];

				if (tile != 0) {
					SDL_Rect srcRect = tileset.get_tile_rect(tile - 1); // Offset by 1 to account for 0 being transparent.
					SDL_Rect destRect = to_rect((position - cameraViewOffset) * tileSize, tileSize);

					SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
				}
			}

		nextRow:
			;
		}
	}
	
	render_internal(scale, camera, secondPassTexture);
}

void Tilemap::render_internal(f32 scale, const Camera& camera, const Texture& texture) {
    glm::vec2 viewport = { VIEWPORT_WIDTH, VIEWPORT_HEIGHT };
    glm::vec2 scaledViewport = viewport / scale;

    SDL_Rect srcRect = to_rect((camera.get_target() * tileset.get_tile_dimensions()) - (scaledViewport / 2.0f), scaledViewport);
    SDL_Rect dstRect = to_rect({ 0, 0 }, viewport);

    // Renders all the layers with z <= 0.
    SDL_RenderCopy(texture.get_renderer(), texture.get(), &srcRect, &dstRect);
}
