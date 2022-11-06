#include <SDL_image.h>

#include "graphics/texture.hpp"
#include "utility/log.hpp"

Texture::Texture(SDL_Renderer* renderer, const u8* filepath)
	: renderer(renderer), filepath(filepath) {
	texture = IMG_LoadTexture(renderer, filepath);
	if (!texture) {
		log::error("IMG_LoadTexture() failed for filepath '%s'.\n%s", filepath, SDL_GetError());

		// Default magenta/black texture.
		u32 pixels[] = { 0xff00ffff, 0x000000ff,
		                 0x000000ff, 0xff00ffff };
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, 2, 2, 32, 8, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
		if (!surface) {
			log::error("Failed to create 'missing texture' surface.\n%s", SDL_GetError());
			return;
		}
		
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		surface = nullptr;

		if (!texture) {
			log::error("Failed to create 'missing texture' texture from a surface.\n%s", SDL_GetError());
			return;
		}
	} else {
		log::info("Loaded texture '%s'.", filepath);
	}

	referencesCount[texture] += 1;
}

Texture::~Texture() {
	if (!texture) {
		return;
	}

	if (referencesCount[texture] == 0) {
		log::warn("Destroying texture '%s' with reference count 0.", filepath);
	} else {
		referencesCount[texture] -= 1;
	}

	if (referencesCount[texture] == 0) {
		SDL_DestroyTexture(texture);
		texture = nullptr;

		log::info("Destroyed texture '%s'.", filepath);
	}

	renderer = nullptr;
}

Texture::Texture(const Texture& other) 
	: texture(other.texture), renderer(other.renderer), filepath(other.filepath) {
	if (texture) {
		referencesCount[texture] += 1;
	}
}

Texture::Texture(Texture&& other)
	: texture(other.texture), renderer(other.renderer), filepath(other.filepath) {
	other.renderer = nullptr;
	other.filepath = nullptr;
	other.texture = nullptr;
}

Texture& Texture::operator=(const Texture& other) {
	renderer = other.renderer;
	filepath = other.filepath;
	texture = other.texture;

	if (texture) {
		referencesCount[texture] += 1;
	}

	return *this;
}

Texture& Texture::operator=(Texture&& other) {
	renderer = other.renderer;
	other.renderer = nullptr;

	filepath = other.filepath;
	other.filepath = nullptr;

	texture = other.texture;
	other.texture = nullptr;

	return *this;
}
