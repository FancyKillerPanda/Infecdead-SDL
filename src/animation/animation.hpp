#pragma once

#include <SDL.h>

#include "common.hpp"

class Animation {
public:
	virtual bool update() = 0;
	virtual void render(SDL_Renderer* renderer, f64 deltaTime) = 0;
};
