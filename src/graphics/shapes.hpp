#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

namespace shapes {

	void draw_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour);
	void fill_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour);

	void draw_circle(SDL_Renderer* renderer, glm::vec2 centre, s32 radius, s32 width, SDL_Color colour);
	void fill_circle(SDL_Renderer* renderer, glm::vec2 centre, s32 radius, SDL_Color colour);

	void fill_rounded_rectangle(SDL_Renderer* renderer, glm::vec4 box, s32 cornerRadius, SDL_Color colour);

}
