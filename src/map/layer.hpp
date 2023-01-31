#pragma once

#include <vector>

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
	u32 layerWidth = 0;
	u32 layerHeight = 0;

	s32 z = 0;
};
