#include "animation/fade.hpp"
#include "graphics/shapes.hpp"
#include "utility/utility.hpp"

FadeAnimation::FadeAnimation(glm::vec4 box, SDL_Color initialColour, SDL_Color targetColour, u32 targetUpdateCount)
	: box(box), initialColour(colour_to_vec4(initialColour)), targetColour(colour_to_vec4(targetColour)), currentUpdateCount(1), targetUpdateCount(targetUpdateCount) {
}

bool FadeAnimation::update() {
	currentUpdateCount += 1;
	if (currentUpdateCount == targetUpdateCount) {
		return true;
	}

	return false;
}

void FadeAnimation::render(SDL_Renderer* renderer, f64 deltaTime) {
	f64 progress = (((f64) currentUpdateCount + deltaTime) / (f64) targetUpdateCount);
	glm::vec4 currentColour = initialColour + ((targetColour - initialColour) * (f32) progress);

	shapes::fill_rectangle(renderer, box, vec4_to_colour(currentColour));
}
