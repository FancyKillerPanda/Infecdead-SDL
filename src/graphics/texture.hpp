#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <SDL.h>

#include "common.hpp"

class Texture {
public:
	Texture() = default;
	Texture(SDL_Renderer* renderer, const u8* filepath);
	Texture(SDL_Renderer* renderer, SDL_Surface* surface);
	Texture(SDL_Renderer* renderer, SDL_Texture* texture);
	~Texture();

	Texture(const Texture& other);
	Texture(Texture&& other);
	Texture& operator=(const Texture& other);
	Texture& operator=(Texture&& other);

	SDL_Texture* get() const { return texture; }
	SDL_Renderer* get_renderer() const { return renderer; }

	void render(glm::vec2 position);

public:
	glm::vec2 dimensions;

private:
	bool init_missing_texture();
	void init_dimensions();

private:
	// TODO(fkp): Maybe cache filepaths?
	inline static std::unordered_map<SDL_Texture*, u32> referencesCount {};

	SDL_Renderer* renderer = nullptr;
	const u8* filepath = nullptr;
	SDL_Texture* texture = nullptr;
};
