#include <SDL.h>

#include "map/layer.hpp"
#include "utility/utility.hpp"

TilemapLayer::TilemapLayer(json layer) {
	if (layer["type"] == "tilelayer") {
		type = TilemapLayerType::TileLayer;

		data = layer["data"];
		layerDimensions = { layer["width"], layer["height"] };

		for (json property : layer["properties"]) {
			if (property["name"] == "z") {
				z = property["value"];
			}
		}
	} else {
		// TODO(fkp): Implement
	}
}

void TilemapLayer::render_to_texture(Texture& texture, const Tileset& tileset, glm::vec2 tileDimensions) const {
	SDL_SetRenderTarget(texture.get_renderer(), texture.get());
	// SDL_SetRenderDrawBlendMode(texture.get_renderer(), SDL_BLENDMODE_BLEND);
	
	glm::vec2 currentCoordinates = { 0, 0 };
	const Texture& tilesetTexture = tileset.get_texture();

	for (u8 tile : get_data()) {
		// Data is 0 for transparent tiles.
		if (tile != 0) {
			SDL_Rect srcRect = tileset.get_tile_rect(tile - 1); // Offset by 1 to account for 0 being transparent.
			SDL_Rect destRect = to_rect(currentCoordinates * tileDimensions, tileDimensions);

			SDL_RenderCopy(tilesetTexture.get_renderer(), tilesetTexture.get(), &srcRect, &destRect);
		}

		currentCoordinates.x += 1;
		if (currentCoordinates.x == layerDimensions.x) {
			currentCoordinates.x = 0;
			currentCoordinates.y += 1;
		}
	}

	SDL_SetRenderTarget(texture.get_renderer(), nullptr);
}
