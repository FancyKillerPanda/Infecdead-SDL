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
			SDL_Rect textRect = { 0, 0, 0, 0 };
			SDL_Rect textureRect = { 0, 0, 0, 0 };
			
			if (i < texts.size()) {
				textRect = to_rect(currentButtonPosition - (texts[i].get_texture().dimensions / 2.0f), texts[i].get_texture().dimensions);
			}
			if (i < textures.size()) {
				textureRect = to_rect(currentButtonPosition - (textures[i].dimensions / 2.0f), textures[i].dimensions);
			}

			if (SDL_HasIntersection(&mouseRect, &textRect) || SDL_HasIntersection(&mouseRect, &textureRect)) {
				hoverIndex = i;
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

void ButtonGroup::default_render_function(ButtonGroup& buttons, u32 currentButton, glm::vec2 position) {
	if (buttons.textures.size() > 0) {
		log::warn("Attempting to use default button renderer with textures. Define your own!");
		return;
	}

	if (currentButton >= buttons.texts.size()) {
		log::error("Current button is larger than the number of buttons.");
		return;
	}
	
	SDL_Color newColour = DEFAULT_BASE_COLOUR;
	if (buttons.pressedIndex != -1) {
		if (buttons.pressedIndex == currentButton) {
			newColour = DEFAULT_PRESSED_COLOUR;
		}
	} else {
		if (buttons.hoverIndex == currentButton) {
			newColour = DEFAULT_HOVER_COLOUR;
		}
	}

	buttons.texts[currentButton].change_colour(newColour);
	buttons.texts[currentButton].render(position - (buttons.texts[currentButton].get_texture().dimensions / 2.0f));
}
