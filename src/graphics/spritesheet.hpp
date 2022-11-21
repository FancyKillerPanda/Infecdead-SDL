#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

#include "common.hpp"
#include "graphics/texture.hpp"

class Spritesheet {
public:
	Spritesheet(Texture texture, glm::vec2 subrectDimensions, u32 numberOfSubrects, u32 subrectsPerRow);

	void render(glm::vec2 position, glm::vec2 dimensions, f64 rotation = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void set_current_subrect_index(u32 index);
	SDL_Rect get_current_subrect();
	
private:
	Texture texture {};

	glm::vec2 subrectDimensions {};
	u32 numberOfSubrects = 0;
	u32 subrectsPerRow = 0;

	u32 currentSubrectIndex = 0;
};
