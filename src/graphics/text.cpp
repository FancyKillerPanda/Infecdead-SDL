#include <string>

#include "text.hpp"

Text::Text(SDL_Renderer* renderer, TTF_Font* font, const u8* message, SDL_Color colour) 
	: renderer(renderer), font(font), message(message) {
	change_colour(colour);
}

void Text::render(glm::vec2 position) {
	texture.render(position);
}

void Text::change_colour(SDL_Color newColour) {
	colour = newColour;

	SDL_Surface* surface = TTF_RenderUTF8_Solid_Wrapped(font, message, colour, 0);
	texture = Texture { renderer, surface };
	SDL_FreeSurface(surface);
}

/*
void Text::size_wrapped(TTF_Font* font, const u8* message, s32* width, s32* height) {
	std::string string { message };

	u32 lineStart = 0;
	u32 lineCurrent = 0;
	s32 longestLineWidth = 0;
	u32 numberOfLines = 1;

	for (char character : string) {
		lineCurrent += 1;
		if (character == '\n') {
			std::string line = string.substr(lineStart, lineCurrent - lineStart - 1);

			numberOfLines += 1;
			lineStart = lineCurrent;

			TTF_SizeUTF8(font, line.c_str(), width, height);
			if (*width > longestLineWidth) {
				longestLineWidth = *width;
			}
		}
	}

	if (numberOfLines == 1) {
		TTF_SizeUTF8(font, message, &longestLineWidth, height);
	}

	*width = longestLineWidth;
	*height = numberOfLines * TTF_FontLineSkip(font);
}
*/
