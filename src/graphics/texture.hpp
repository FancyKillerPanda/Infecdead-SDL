#pragma once

#include <unordered_map>

#include <SDL.h>

#include "common.hpp"

class Texture {
public:
	Texture(SDL_Renderer* renderer, const u8* filepath);
	~Texture();

	Texture(const Texture& other);
	Texture(Texture&& other);
	Texture& operator=(const Texture& other);
	Texture& operator=(Texture&& other);

	SDL_Texture* get() { return texture; }
	SDL_Renderer* get_renderer() { return renderer; }

private:
	// TODO(fkp): Maybe cache filepaths?
	inline static std::unordered_map<SDL_Texture*, u32> referencesCount {};

	SDL_Renderer* renderer = nullptr;
	const u8* filepath = nullptr;
	SDL_Texture* texture = nullptr;
};
