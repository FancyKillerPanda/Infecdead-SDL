#include "game/camera.hpp"
#include "map/tilemap.hpp"

Camera::Camera(const Tilemap& tilemap, const Character& characterToFollow)
    : tilemap(tilemap), characterToFollow(characterToFollow) {
    update();
}

void Camera::update() {
    glm::vec2 viewport = { VIEWPORT_WIDTH, VIEWPORT_HEIGHT };
    glm::vec2 worldSpaceViewport = viewport / tilemap.get_default_scale();
    glm::vec2 mapDimensionsWorldSpace = tilemap.get_map_dimensions() * tilemap.get_tileset().get_tile_dimensions();

    targetWorldPosition = characterToFollow.get_world_position();
    // printf("%f, %f\n", targetWorldPosition.x, targetWorldPosition.y);
    targetWorldPosition = glm::clamp(targetWorldPosition, worldSpaceViewport / 2.0f, mapDimensionsWorldSpace - (worldSpaceViewport / 2.0f));
}
