#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

#include "animation/animation.hpp"

class LogoAnimation : public Animation {
public:
	void update() override;
	void render(SDL_Renderer* renderer, f64 deltaTime) override;

private:
	u32 currentAlpha = 0;
};
