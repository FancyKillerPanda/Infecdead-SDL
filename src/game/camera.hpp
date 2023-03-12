#pragma once

#include <glm/glm.hpp>

#include "game/entity/character.hpp"

class Tilemap;

class Camera {
public:
    Camera(const Tilemap& tilemap, const Character& characterToFollow);
    void update();

    const glm::vec2& get_target() const { return targetWorldPosition; }

private:
    const Tilemap& tilemap;
    const Character& characterToFollow;

    glm::vec2 targetWorldPosition;
};
