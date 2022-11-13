#include <string>

#include "utility/utility.hpp"
#include "graphics/text.hpp"

Text::Text(SDL_Renderer* renderer, TTF_Font* font, const u8* message, SDL_Color colour) 
	: renderer(renderer), font(font), message(message) {
	change_colour(colour);
}

void Text::render(glm::vec2 centre) {
	texture.render(centre - (texture.dimensions / 2.0f));
}

void Text::change_colour(SDL_Color newColour) {
	if (colour == newColour) {
		return;
	}
	
	colour = newColour;

	SDL_Surface* surface = TTF_RenderUTF8_Solid_Wrapped(font, message, colour, 0);
	texture = Texture { renderer, surface };
	SDL_FreeSurface(surface);
}
