#pragma once

#include <BlockType.hpp>

class Block {
	public:
		BlockType* type;
		sf::Vector2i position;
		int rotation_state;
		Block();
		std::vector<sf::Vector2i> get_tiles();
};