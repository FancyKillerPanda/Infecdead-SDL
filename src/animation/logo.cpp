#include "animation/logo.hpp"
#include "graphics/shapes.hpp"

LogoAnimation::LogoAnimation() {
	
}

bool LogoAnimation::update() {
	u32 ALPHA_INCREASE_RATE = 3;
	currentAlpha += ALPHA_INCREASE_RATE;
	
	u32 delay = 100 * ALPHA_INCREASE_RATE;
	return currentAlpha >= (255 * 3) + delay;
}

void LogoAnimation::render(SDL_Renderer* renderer, f64 deltaTime) {
	glm::vec2 unit { 50, 50 };
	glm::vec2 unit3 = unit * 3.0f;
	glm::vec2 unit6 = unit * 6.0f;
	glm::vec2 unit9 = unit * 9.0f;

	glm::vec2 centre { (VIEWPORT_WIDTH / 2) - unit3.x, (VIEWPORT_HEIGHT / 2) + unit3.y };

	s32 LINE_WIDTH = 16;

	s32 alpha = currentAlpha;
	SDL_Color white = { 255, 255, 255, (u8) SDL_clamp(alpha, 0, 255)};
	alpha -= 255;
	SDL_Color red = { 255, 0, 0, (u8) SDL_clamp(alpha, 0, 255) };
	alpha -= 255;
	SDL_Color lime = { 108, 255, 0, (u8) SDL_clamp(alpha, 0, 255) };

	using namespace shapes;

	// F
	draw_line(renderer, { centre.x - unit3.x, centre.y + unit3.y }, { centre.x + unit3.x, centre.y - unit3.y } , LINE_WIDTH, white);
	draw_line(renderer, { centre.x + unit3.x, centre.y - unit3.y }, { centre.x + unit6.x, centre.y } , LINE_WIDTH, white);
	draw_line(renderer, { centre }, { centre.x + unit3.x, centre.y + unit3.y } , LINE_WIDTH, white);

	// K
	draw_line(renderer, { centre.x + unit3.x, centre.y + unit3.y }, { centre.x + unit3.x, centre.y - unit9.y } , LINE_WIDTH, red);
	draw_line(renderer, { centre.x + unit3.x, centre.y - unit3.y }, { centre.x + unit6.x, centre.y - unit6.y } , LINE_WIDTH, red);

	// P
	draw_line(renderer, { centre.x + unit3.x, centre.y - unit9.y }, { centre.x + unit6.x, centre.y - unit6.y } , LINE_WIDTH, lime);
}
