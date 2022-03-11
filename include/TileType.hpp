#pragma once

#include <SFML/Graphics/Rect.hpp>

#include <Config.hpp>

class TileType {
	public:
		sf::IntRect texture_rect;
		sf::IntRect ghost_texture_rect;
		TileType(sf::IntRect _texture_rect, sf::IntRect _ghost_texture_rect);
};