#include "common.hpp"
#include "graphics/shapes.hpp"
#include "utility/utility.hpp"

#define SAVE_CURRENT_COLOUR() \
	Uint8 r, g, b, a; \
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)

#define RESET_COLOUR() SDL_SetRenderDrawColor(renderer, r, g, b, a)

namespace shapes {
	
	void draw_rectangle(SDL_Renderer* renderer, glm::vec2 position, glm::vec2 dimensions, SDL_Color colour) {
		SAVE_CURRENT_COLOUR();

		SDL_Rect rect = to_rect(position, dimensions);
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderDrawRect(renderer, &rect);

		RESET_COLOUR();
	}

	void fill_rectangle(SDL_Renderer* renderer, glm::vec2 position, glm::vec2 dimensions, SDL_Color colour) {
		SAVE_CURRENT_COLOUR();

		SDL_Rect rect = to_rect(position, dimensions);
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &rect);

		RESET_COLOUR();
	}

}

#undef RESET_COLOUR
#undef SAVE_CURRENT_COLOUR
