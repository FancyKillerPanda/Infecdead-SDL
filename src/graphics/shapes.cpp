#include <vector>

#include "common.hpp"
#include "graphics/shapes.hpp"
#include "utility/log.hpp"
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

	void draw_circle(SDL_Renderer* renderer, glm::vec2 centre, s32 radius, s32 width, SDL_Color colour) {
		SDL_Texture* texture = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, radius * 2, radius * 2);
		if (!texture) {
			log::error("Failed to create texture for circle drawing.\n%s", SDL_GetError());
			return;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		SDL_BlendMode oldBlendMode;
		SDL_GetRenderDrawBlendMode(renderer, &oldBlendMode);
		SDL_Texture* oldRenderTarget = SDL_GetRenderTarget(renderer);

		SDL_SetRenderTarget(renderer, texture);
		fill_circle(renderer, glm::vec2 { radius, radius }, radius, colour);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		fill_circle(renderer, glm::vec2 { radius, radius }, radius - width, SDL_Color { 0, 0, 0, 0 });
		SDL_SetRenderTarget(renderer, oldRenderTarget);

		SDL_Rect rect = {
			(s32) (centre.x - radius),
			(s32) (centre.y - radius),
			radius * 2,
			radius * 2,
		};
		SDL_RenderCopy(renderer, texture, nullptr, &rect);

		SDL_DestroyTexture(texture);
		SDL_SetRenderDrawBlendMode(renderer, oldBlendMode);
	}

	void fill_circle(SDL_Renderer* renderer, glm::vec2 centre, s32 radius, SDL_Color colour) {
		s32 numTriangles = max(radius / 3, 16);
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

	void fill_rounded_rectangle(SDL_Renderer* renderer, glm::vec4 box, s32 cornerRadius, SDL_Color colour) {
		SAVE_CURRENT_COLOUR();

		SDL_Rect horizontalRect = {
			(s32) box.x,
			(s32) (box.y + cornerRadius),
			(s32) box.z,
			(s32) (box.w - (cornerRadius * 2)),
		};

		SDL_Rect verticalRect = {
			(s32) (box.x + cornerRadius),
			(s32) box.y,
			(s32) (box.z - (cornerRadius * 2)),
			(s32) box.w,
		};

		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &horizontalRect);
		SDL_RenderFillRect(renderer, &verticalRect);
		
		// Corners
		fill_circle(renderer, { box.x + cornerRadius, box.y + cornerRadius }, cornerRadius, colour);
		fill_circle(renderer, { box.x + box.z - cornerRadius, box.y + cornerRadius }, cornerRadius, colour);
		fill_circle(renderer, { box.x + cornerRadius, box.y + box.w - cornerRadius }, cornerRadius, colour);
		fill_circle(renderer, { box.x + box.z - cornerRadius, box.y + box.w - cornerRadius }, cornerRadius, colour);

		RESET_COLOUR();
	}

}

#undef RESET_COLOUR
#undef SAVE_CURRENT_COLOUR
