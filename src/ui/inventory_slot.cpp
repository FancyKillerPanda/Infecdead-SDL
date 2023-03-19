#include "game/game.hpp"
#include "ui/inventory_slot.hpp"

InventorySlot::InventorySlot(Game& game) 
	: baseTexture(game.get_renderer(), "res/ui/inventory_slot_background.png"),
	  selectedTexture(game.get_renderer(), "res/ui/inventory_slot_selected.png") {
}

void InventorySlot::render(glm::vec2 position, bool isSelected) {
	Texture& texture = isSelected ? selectedTexture : baseTexture;
	texture.render(position);
}
