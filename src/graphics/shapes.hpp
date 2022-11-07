#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

namespace shapes {

	void draw_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour);
	void fill_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour);

}
