#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

#include "animation/animation.hpp"

// TODO(fkp): Support fades to/from a texture.
class FadeAnimation : public Animation {
public:
	FadeAnimation(glm::vec4 box, SDL_Color initialColour, SDL_Color targetColour, u32 targetUpdateCount);

	bool update() override;
	void render(SDL_Renderer* renderer, f64 deltaTime) override;

private:
	glm::vec4 box;

	glm::vec4 initialColour;
	glm::vec4 targetColour;

	u32 currentUpdateCount;
	u32 targetUpdateCount;
};
