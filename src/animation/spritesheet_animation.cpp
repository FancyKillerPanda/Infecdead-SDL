#include "animation/spritesheet_animation.hpp"
#include "graphics/spritesheet.hpp"

SpritesheetAnimation::SpritesheetAnimation(Spritesheet& spritesheet, std::vector<u32> animationOrder, u32 targetUpdateCount) 
	: spritesheet(spritesheet), animationOrder(animationOrder), targetUpdateCount(targetUpdateCount) {
}

void SpritesheetAnimation::update() {
	currentUpdateCount += 1;
	if (currentUpdateCount >= targetUpdateCount) {
		currentUpdateCount = 0;
		next_animation_frame();
	}
}

void SpritesheetAnimation::next_animation_frame() {
	currentAnimationIndex += 1;

	if (animationOrder.size() == 0) {
		currentAnimationIndex %= spritesheet.numberOfSubrects;
		spritesheet.set_current_subrect_index(currentAnimationIndex);
	} else {
		currentAnimationIndex %= animationOrder.size();
		spritesheet.set_current_subrect_index(animationOrder[currentAnimationIndex]);
	}
}
