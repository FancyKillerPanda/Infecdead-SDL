#include "animation/logo.hpp"
#include "graphics/shapes.hpp"

LogoAnimation::LogoAnimation() {
	
}

bool LogoAnimation::update() {
	return false;
}

void LogoAnimation::render(SDL_Renderer* renderer, f64 deltaTime) {
	glm::vec2 centre { VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2 };
	glm::vec2 unit { 50, 50 };
	s32 lineWidth = 10;

	using namespace shapes;
	draw_line(renderer, centre - (unit * 3.0f), centre + (unit * 3.0f), lineWidth, SDL_Color { 255, 255, 255, 255 });
}
