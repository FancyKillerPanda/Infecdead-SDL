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
			if (animation->isFinished) {
				i += 1;
				continue;
			}
			
			animation->update();
			if (animation->isFinished) {
				for (Animation* next : animation->spawnNext) {
					currentAnimations.push_back(next);
				}
				
				if (!animation->renderAfterFinished) {
					delete currentAnimations[i];
					currentAnimations.erase(currentAnimations.begin() + i);
				} else {
					i += 1;
				}
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
