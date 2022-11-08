#include <string>

#include "graphics/text.hpp"

Text::Text(SDL_Renderer* renderer, TTF_Font* font, const u8* message, SDL_Color colour) 
	: renderer(renderer), font(font), message(message) {
	change_colour(colour);
}

void Text::render(glm::vec2 position) {
	texture.render(position);
}

void Text::change_colour(SDL_Color newColour) {
	if (colour.r == newColour.r && colour.g == newColour.g && colour.b == newColour.b && colour.a == newColour.a) {
		return;
	}
	
	colour = newColour;

	SDL_Surface* surface = TTF_RenderUTF8_Solid_Wrapped(font, message, colour, 0);
	texture = Texture { renderer, surface };
	SDL_FreeSurface(surface);
}
