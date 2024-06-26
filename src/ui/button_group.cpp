#include "ui/button_group.hpp"
#include "utility/log.hpp"
#include "utility/utility.hpp"

constexpr SDL_Color DEFAULT_BASE_COLOUR = { 0, 0, 0, 255 };
constexpr SDL_Color DEFAULT_HOVER_COLOUR = { 191, 191, 191, 255 };
constexpr SDL_Color DEFAULT_PRESSED_COLOUR = { 127, 127, 127, 255 };

ButtonGroup::ButtonGroup(SDL_Renderer* renderer, std::vector<Text>&& texts, std::vector<Texture>&& textures)
	: renderer(renderer), texts(texts), textures(textures) {
	if (texts.size() != 0 && textures.size() != 0 && texts.size() != textures.size()) {
		log::warn("Button group must have the same number of texts and textures.");
	}
}

s32 ButtonGroup::handle_input(const SDL_Event& event) {
	if (lastRenderCentre == glm::vec2 { -1, -1 }) {
		// Skip this frame.
		return -1;
	}
	
	u32 numItems = max(texts.size(), textures.size());
	
	switch (event.type) {
	case SDL_MOUSEMOTION: {
		hoverIndex = -1;
		SDL_Rect mouseRect = { event.motion.x, event.motion.y, 1, 1 };
		glm::vec2 currentButtonPosition = lastRenderCentre - (glm::vec2 { lastRenderSpacing.x * ((numItems - 1) / 2.0), lastRenderSpacing.y * ((numItems - 1) / 2.0) });
		
		for (u32 i = 0; i < numItems; i++) {
			std::vector<glm::vec4> hitboxes = get_hitboxes_function(*this, i, currentButtonPosition);
			for (const glm::vec4& hitbox : hitboxes) {
				SDL_Rect buttonRect = to_rect(hitbox);
				
				if (SDL_HasIntersection(&mouseRect, &buttonRect)) {
					hoverIndex = i;
					break;
				}
			}

			currentButtonPosition += lastRenderSpacing;
		}
		
		break;
	}

	case SDL_MOUSEBUTTONDOWN:
		pressedIndex = hoverIndex;
		break;

	case SDL_MOUSEBUTTONUP:
		s32 oldPressedIndex = pressedIndex;
		pressedIndex = -1;
		
		return oldPressedIndex;
	}

	return -1;
}

void ButtonGroup::render(glm::vec2 centre, glm::vec2 spacing) {
	u32 numItems = max(texts.size(), textures.size());
	glm::vec2 currentButtonPosition = centre - (glm::vec2 { spacing.x * ((numItems - 1) / 2.0), spacing.y * ((numItems - 1) / 2.0) });
	
	for (u32 i = 0; i < numItems; i++) {
		render_function(*this, i, currentButtonPosition);
		currentButtonPosition += spacing;
	}

	lastRenderCentre = centre;
	lastRenderSpacing = spacing;
}

void ButtonGroup::reset() {
	hoverIndex = -1;
	pressedIndex = -1;
}

SDL_Color ButtonGroup::get_current_colour(s32 currentButton, SDL_Color baseColour, SDL_Color hoverColour, SDL_Color pressedColour) {
	if (get_pressed_index() != -1) {
		if (get_pressed_index() == currentButton) {
			return pressedColour;
		}
	} else {
		if (get_hover_index() == currentButton) {
			return hoverColour;
		}
	}

	return baseColour;
}

void ButtonGroup::default_render_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
	if (buttons.textures.size() > 0) {
		log::warn("Attempting to use default button renderer with textures. Define your own!");
		return;
	}

	if (currentButton >= buttons.texts.size()) {
		log::error("Current button is larger than the number of buttons.");
		return;
	}
	
	SDL_Color newColour = buttons.get_current_colour(currentButton, DEFAULT_BASE_COLOUR, DEFAULT_HOVER_COLOUR, DEFAULT_PRESSED_COLOUR);
	buttons.texts[currentButton].set_colour(newColour);
	buttons.texts[currentButton].render(position);
}

std::vector<glm::vec4> ButtonGroup::default_get_hitboxes_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
	if (buttons.textures.size() > 0) {
		log::warn("Attempting to use default button get hitboxes function with textures. Define your own!");
		return {};
	}

	if (currentButton >= buttons.texts.size()) {
		log::error("Current button is larger than the number of buttons.");
		return {};
	}
	
	glm::vec2 dimensions = buttons.texts[currentButton].get_texture().dimensions;
	return { glm::vec4 { position - (dimensions / 2.0f), dimensions } };
}
