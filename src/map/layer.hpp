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

	const std::vector<u8>& get_data() const { return data; }
	const glm::vec2& get_dimensions() const { return layerDimensions; };
	s32 get_z_index() const { return z; }

public:
	TilemapLayerType type;

private:
	std::vector<u8> data;
	glm::vec2 layerDimensions; // Number of columns and rows.

	s32 z = 0;
};
