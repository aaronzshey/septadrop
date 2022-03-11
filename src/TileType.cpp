#include <SFML/Graphics/Rect.hpp>

#include <TileType.hpp>

TileType::TileType(sf::IntRect _texture_rect, sf::IntRect _ghost_texture_rect) {
	texture_rect = _texture_rect;
	ghost_texture_rect = _ghost_texture_rect;
}