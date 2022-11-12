#pragma once

#include <SDL.h>

#include "common.hpp"
#include "animation/animation.hpp"

class Game;

class State {
public:
	State(Game& game) : game(game) {};

	virtual void handle_input(const SDL_Event& event) {}
	virtual void update() {}
	virtual void render(f64 deltaTime) {}

protected:
	virtual void animate() {
		for (u32 i = 0; i < currentAnimations.size(); ) {
			Animation* animation = currentAnimations[i];
			if (animation->update()) {
				delete currentAnimations[i];
				currentAnimations.erase(currentAnimations.begin() + i);
			} else {
				i += 1;
			}
		}
	}

public:
	std::vector<Animation*> currentAnimations {};
	
protected:
	Game& game;
};
