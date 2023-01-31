#include "map/layer.hpp"

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
