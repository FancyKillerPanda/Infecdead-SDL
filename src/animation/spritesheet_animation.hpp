#pragma once

#include <vector>

#include "animation/animation.hpp"

class Spritesheet;

class SpritesheetAnimation : public Animation {
public:
	SpritesheetAnimation(Spritesheet& spritesheet, std::vector<u32> animationOrder, u32 targetUpdateCount);

	void update() override;
	void render(SDL_Renderer* renderer, f64 deltaTime) override {} // render() is not needed, Spritesheet does its own rendering.

private:
	void next_animation_frame();

private:
	Spritesheet& spritesheet;

	std::vector<u32> animationOrder; // Can be empty to represent animation through sequential frames.
	u32 currentAnimationIndex = 0;

	u32 currentUpdateCount = 0;
	u32 targetUpdateCount = 0;
};
