#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

namespace shapes {

	void draw_rectangle(SDL_Renderer* renderer, glm::vec2 position, glm::vec2 dimensions, SDL_Color colour);
	void fill_rectangle(SDL_Renderer* renderer, glm::vec2 position, glm::vec2 dimensions, SDL_Color colour);

}
