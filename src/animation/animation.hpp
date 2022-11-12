#pragma once

#include <vector>

#include <SDL.h>

#include "common.hpp"

class Animation {
public:
	virtual ~Animation() = default;

	virtual void update() = 0;
	virtual void render(SDL_Renderer* renderer, f64 deltaTime) = 0;

public:
	std::vector<Animation*> spawnNext {};

	bool isFinished = false;
	bool renderAfterFinished = false;
};
