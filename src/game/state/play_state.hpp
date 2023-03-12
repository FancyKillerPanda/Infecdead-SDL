#pragma once

#include "game/camera.hpp"
#include "game/entity/player.hpp"
#include "game/state/state.hpp"
#include "map/tileset.hpp"
#include "map/tilemap.hpp"

class PlayState : public State {
public:
	PlayState(Game& game);

	void handle_input(const SDL_Event& event) override;
	void update() override;
	void render(f64 deltaTime) override;

private:
	Player player;
	Tileset tileset;

    Tilemap worldMap;
    Camera camera;
};
