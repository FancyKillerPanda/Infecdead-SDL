#pragma once

#include <glm/glm.hpp>

#include <SDL.h>
#include <SDL_ttf.h>

#include "common.hpp"
#include "graphics/texture.hpp"

class Text {
public:
	Text() = default;
	Text(SDL_Renderer* renderer, TTF_Font* font, const u8* message, SDL_Color colour);

	void render(glm::vec2 centre);
	void change_colour(SDL_Color newColour);

	const Texture& get_texture() { return texture; }

private:
	const u8* message = nullptr;

	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;

	Texture texture;
	SDL_Color colour;
};
