#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "graphics/text.hpp"
#include "graphics/texture.hpp"

class ButtonGroup {
	using RenderFunctionSignature = void (*)(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);
	using GetHitboxesFunctionSignature = std::vector<glm::vec4> (*)(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);
	
public:
	ButtonGroup() = default;
	ButtonGroup(SDL_Renderer* renderer, std::vector<Text>&& texts, std::vector<Texture>&& textures);

	s32 handle_input(const SDL_Event& event);
	void render(glm::vec2 centre, glm::vec2 spacing);
	
	SDL_Color get_current_colour(s32 currentButton, SDL_Color baseColour, SDL_Color hoverColour, SDL_Color pressedColour);
	
	void set_render_function(RenderFunctionSignature func) { render_function = func; }
	void set_get_hitboxes_function(GetHitboxesFunctionSignature func) { get_hitboxes_function = func; }

	SDL_Renderer* get_renderer() { return renderer; }

	void reset();
	s32 get_hover_index() { return hoverIndex; }
	s32 get_pressed_index() { return pressedIndex; }
	
private:
	static void default_render_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);
	static std::vector<glm::vec4> default_get_hitboxes_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position);

public:
	std::vector<Text> texts;
	std::vector<Texture> textures;

private:
	SDL_Renderer* renderer = nullptr;
	glm::vec2 lastRenderCentre = { -1, -1 };
	glm::vec2 lastRenderSpacing = { -1, -1 };

	s32 hoverIndex = -1;
	s32 pressedIndex = -1;

	RenderFunctionSignature render_function = default_render_function;
	GetHitboxesFunctionSignature get_hitboxes_function = default_get_hitboxes_function;
};
