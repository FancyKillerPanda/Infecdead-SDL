#include "animation/spritesheet_animation.hpp"
#include "graphics/spritesheet.hpp"
#include "utility/log.hpp"
#include "utility/utility.hpp"

Spritesheet::Spritesheet(Texture texture, glm::vec2 subrectDimensions, u32 numberOfSubrects, u32 subrectsPerRow, SpritesheetAnimation* animation)
	: texture(texture), subrectDimensions(subrectDimensions), numberOfSubrects(numberOfSubrects), subrectsPerRow(subrectsPerRow), animation(animation) {
	log::info("Initialised spritesheet with %u subrects (%u per row).", numberOfSubrects, subrectsPerRow);
}

void Spritesheet::update() {
	if (animation) {
		animation->update();
	}
}

void Spritesheet::render(glm::vec2 position, glm::vec2 dimensions, f64 rotation, SDL_RendererFlip flip) {
	SDL_Rect rect = to_rect(position - (dimensions / 2.0f), dimensions);
	SDL_Rect subrect = get_current_subrect();

	SDL_RenderCopyEx(texture.get_renderer(), texture.get(), &subrect, &rect, rotation, nullptr, flip);
}

void Spritesheet::set_current_subrect_index(u32 index) {
	if (currentSubrectIndex >= numberOfSubrects) {
		log::error("Attempting to set subrect index out of range.");
		return;
	}

	currentSubrectIndex = index;
}

SDL_Rect Spritesheet::get_current_subrect() {
	u32 row = currentSubrectIndex / subrectsPerRow;
	u32 col = currentSubrectIndex % subrectsPerRow;

	return to_rect(glm::vec2 { row, col } * subrectDimensions, subrectDimensions);
}
