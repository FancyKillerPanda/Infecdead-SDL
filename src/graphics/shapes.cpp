#include <vector>

#include "common.hpp"
#include "graphics/shapes.hpp"
#include "utility/utility.hpp"

#define SAVE_CURRENT_COLOUR() \
	Uint8 r, g, b, a; \
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)

#define RESET_COLOUR() SDL_SetRenderDrawColor(renderer, r, g, b, a)

namespace shapes {
	
	void draw_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour) {
		SAVE_CURRENT_COLOUR();

		SDL_Rect rect = to_rect(box);
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderDrawRect(renderer, &rect);

		RESET_COLOUR();
	}

	void fill_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour) {
		SAVE_CURRENT_COLOUR();

		SDL_Rect rect = to_rect(box);
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &rect);

		RESET_COLOUR();
	}

	void fill_circle(SDL_Renderer* renderer, glm::vec2 centre, s32 radius, SDL_Color colour) {
		s32 numTriangles = radius / 3;
		std::vector<SDL_Vertex> vertices {};

		// TODO(fkp): Optimise with an index array.
		for (s32 i = 0; i < numTriangles; i++) {
			vertices.push_back(SDL_Vertex {
				SDL_FPoint { centre.x, centre.y },
				colour,
				SDL_FPoint { 0, 0 }
			});
			
			vertices.push_back(SDL_Vertex {
				SDL_FPoint { centre.x + (float) (radius * cos(i * 2.0 * M_PI / numTriangles)), centre.y + (float) (radius * sin(i * 2.0 * M_PI / numTriangles)) },
				colour,
				SDL_FPoint { 0, 0 }
			});

			vertices.push_back(SDL_Vertex {
				SDL_FPoint { centre.x + (float) (radius * cos((i + 1) * 2.0 * M_PI / numTriangles)), centre.y + (float) (radius * sin((i + 1) * 2.0 * M_PI / numTriangles)) },
				colour,
				SDL_FPoint { 0, 0 }
			});
		}

		SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
	}

}

#undef RESET_COLOUR
#undef SAVE_CURRENT_COLOUR
