#include "game/camera.hpp"
#include "map/tilemap.hpp"

Camera::Camera(const Tilemap& tilemap, const Character& characterToFollow)
    : tilemap(tilemap), characterToFollow(characterToFollow) {
    update();
}

void Camera::update() {
    glm::vec2 worldSpaceViewport = get_world_space_viewport();

    targetWorldPosition = characterToFollow.get_world_position();
    targetWorldPosition = glm::clamp(targetWorldPosition, worldSpaceViewport / 2.0f, tilemap.get_map_dimensions() - (worldSpaceViewport / 2.0f));
}

glm::vec2 Camera::get_view_offset() const {
    glm::vec2 worldSpaceViewport = get_world_space_viewport();
    return targetWorldPosition - (worldSpaceViewport / 2.0f);
}

glm::vec2 Camera::get_world_space_viewport() const {
    glm::vec2 viewport = { VIEWPORT_WIDTH, VIEWPORT_HEIGHT };
    glm::vec2 worldSpaceViewport = (viewport / tilemap.get_default_scale()) / tilemap.get_tileset().get_tile_dimensions();

    return worldSpaceViewport;
}
