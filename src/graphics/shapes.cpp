#include <vector>

#include "common.hpp"
#include "graphics/shapes.hpp"
#include "utility/log.hpp"
#include "utility/utility.hpp"

#define SAVE_RENDER_DATA() \
	Uint8 r, g, b, a; \
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a); \
	SDL_BlendMode oldBlendMode; \
	SDL_GetRenderDrawBlendMode(renderer, &oldBlendMode); \
	SDL_Texture* oldRenderTarget = SDL_GetRenderTarget(renderer);

#define RESET_RENDER_DATA() \
	SDL_SetRenderDrawBlendMode(renderer, oldBlendMode); \
	SDL_SetRenderTarget(renderer, oldRenderTarget); \
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

namespace shapes {
	
	void draw_line(SDL_Renderer* renderer, glm::vec2 pointA, glm::vec2 pointB, s32 width, SDL_Color colour) {
		SAVE_RENDER_DATA();

		f32 angle = atan2(pointA.y - pointB.y, pointA.x - pointB.x);
		f32 sinAngle = sin(angle);
		f32 cosAngle = cos(angle);

		std::vector<SDL_Vertex> vertices {};
		std::vector<glm::vec2> positions {
			{ pointA.x - (sinAngle * (width / 2.0f)), pointA.y + (cosAngle * (width / 2.0f)) },
			{ pointB.x - (sinAngle * (width / 2.0f)), pointB.y + (cosAngle * (width / 2.0f)) },
			{ pointB.x + (sinAngle * (width / 2.0f)), pointB.y - (cosAngle * (width / 2.0f)) },
			
			{ pointB.x + (sinAngle * (width / 2.0f)), pointB.y - (cosAngle * (width / 2.0f)) },
			{ pointA.x + (sinAngle * (width / 2.0f)), pointA.y - (cosAngle * (width / 2.0f)) },
			{ pointA.x - (sinAngle * (width / 2.0f)), pointA.y + (cosAngle * (width / 2.0f)) },
		};

		for (const glm::vec2& position : positions) {
			vertices.push_back(SDL_Vertex { SDL_FPoint { position.x, position.y }, colour, SDL_FPoint { 0, 0 }});
		}

		SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);

		RESET_RENDER_DATA();
	}
	
	void draw_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour) {
		SAVE_RENDER_DATA();

		SDL_Rect rect = to_rect(box);
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderDrawRect(renderer, &rect);

		RESET_RENDER_DATA();
	}

	void fill_rectangle(SDL_Renderer* renderer, glm::vec4 box, SDL_Color colour) {
		SAVE_RENDER_DATA();

		SDL_Rect rect = to_rect(box);
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &rect);

		RESET_RENDER_DATA();
	}

	void draw_circle(SDL_Renderer* renderer, glm::vec2 centre, s32 radius, s32 width, SDL_Color colour) {
		SAVE_RENDER_DATA();
		
		SDL_Texture* texture = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, radius * 2, radius * 2);
		if (!texture) {
			log::error("Failed to create texture for circle drawing.\n%s", SDL_GetError());
			return;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		SDL_SetRenderTarget(renderer, texture);
		fill_circle(renderer, glm::vec2 { radius, radius }, radius, colour);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		fill_circle(renderer, glm::vec2 { radius, radius }, radius - width, SDL_Color { 0, 0, 0, 0 });

		RESET_RENDER_DATA();

		SDL_Rect rect = {
			(s32) (centre.x - radius),
			(s32) (centre.y - radius),
			radius * 2,
			radius * 2,
		};
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
		SDL_DestroyTexture(texture);
	}

	void fill_circle(SDL_Renderer* renderer, glm::vec2 centre, s32 radius, SDL_Color colour, f64 angleStart, f64 angleEnd) {
		s32 numTriangles = max(radius / 3, 16);
		std::vector<SDL_Vertex> vertices {};

		// TODO(fkp): Optimise with an index array.
		for (s32 i = 0; i < numTriangles; i++) {
			f32 angle = angleStart + ((angleEnd - angleStart) * ((f64) i / numTriangles));
			f32 nextAngle = angleStart + ((angleEnd - angleStart) * ((f64) (i + 1) / numTriangles));
			
			vertices.push_back(SDL_Vertex {
				SDL_FPoint { centre.x, centre.y },
				colour,
				SDL_FPoint { 0, 0 }
			});
			
			vertices.push_back(SDL_Vertex {
				SDL_FPoint { centre.x + (float) (radius * cos(angle)), centre.y + (float) (radius * sin(angle)) },
				colour,
				SDL_FPoint { 0, 0 }
			});

			vertices.push_back(SDL_Vertex {
				SDL_FPoint { centre.x + (float) (radius * cos(nextAngle)), centre.y + (float) (radius * sin(nextAngle)) },
				colour,
				SDL_FPoint { 0, 0 }
			});
		}

		SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
	}

	void draw_rounded_rectangle(SDL_Renderer* renderer, glm::vec4 box, s32 cornerRadius, s32 width, SDL_Color colour) {
		SAVE_RENDER_DATA();
		
		SDL_Texture* texture = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, box.z, box.w);
		if (!texture) {
			log::error("Failed to create texture for rounded rectangle drawing.\n%s", SDL_GetError());
			return;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		SDL_SetRenderTarget(renderer, texture);
		fill_rounded_rectangle(renderer, { 0, 0, box.z, box.w }, cornerRadius, colour);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		fill_rounded_rectangle(renderer, { width, width, box.z - (width * 2), box.w - (width * 2) }, cornerRadius, SDL_Color { 0, 0, 0, 0 });

		RESET_RENDER_DATA();

		SDL_Rect rect = to_rect(box);
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
		SDL_DestroyTexture(texture);
	}

	void fill_rounded_rectangle(SDL_Renderer* renderer, glm::vec4 box, s32 cornerRadius, SDL_Color colour) {
		SAVE_RENDER_DATA();

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

		RESET_RENDER_DATA();
	}

}

#undef RESET_RENDER_DATA
#undef SAVE_RENDER_DATA
