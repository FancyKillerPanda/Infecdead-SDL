#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

#include "animation/animation.hpp"

class LogoAnimation : public Animation {
public:
	LogoAnimation();

	bool update() override;
	void render(SDL_Renderer* renderer, f64 deltaTime) override;
};
