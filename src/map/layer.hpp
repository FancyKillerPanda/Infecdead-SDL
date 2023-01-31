#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <nlohmann/json.hpp>

#include "common.hpp"

using json = nlohmann::json;

enum class TilemapLayerType {
	TileLayer,
};

class TilemapLayer {
public:
	TilemapLayer(json layer);
	TilemapLayer(const TilemapLayer&) = delete;
	TilemapLayer& operator=(const TilemapLayer&) = delete;

public:
	TilemapLayerType type;

private:
	std::vector<u8> data;
	glm::vec2 layerDimensions; // Number of columns and rows.

	s32 z = 0;
};
