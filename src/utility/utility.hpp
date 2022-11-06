#pragma once

#include <glm/glm.hpp>

#include <SDL.h>

#include "common.hpp"

inline u32 min(u32 a, u32 b) {
	return a < b ? a : b;
}

inline u32 max(u32 a, u32 b) {
	return a > b ? a : b;
}

inline SDL_Rect to_rect(glm::vec4 box) {
	return SDL_Rect {
		(s32) box.x,
		(s32) box.y,
		(s32) box.z,
		(s32) box.w,
	};
}
