#include <Block.hpp>
#include <BlockType.hpp>
#include <Config.hpp>

Block::Block() {
	type = BlockType::random();
	position = sf::Vector2i(GRID_WIDTH / 2 - type->grid[0].size() / 2, 0);
	rotation_state = 0;
}

std::vector<sf::Vector2i> Block::get_tiles() {
	std::vector<sf::Vector2i>tiles = {};
	for (int y = 0; y < type->grid.size(); y++) {
		for (int x = 0; x < type->grid[y].size(); x++) {
			if (!type->grid[y][x]) {
				continue;
			}
			int rotated_x = x;
			int rotated_y = y;
			if (type->rotate) {
				int center_x = type->grid[0].size() / 2;
				int center_y = type->grid.size() / 2;
				int offset_x = x - center_x;
				int offset_y = y - center_y;
				switch (rotation_state) {
					case 0:
						rotated_x = x;
						rotated_y = y;
						break;
					case 1:
						rotated_x = center_x + offset_y;
						rotated_y = center_y - offset_x;
						break;
					case 2:
						rotated_x = center_x - offset_x;
						rotated_y = center_y - offset_y;
						break;
					case 3:
						rotated_x = center_x - offset_y;
						rotated_y = center_y + offset_x;
						break;
					default:
						rotation_state %= 4;
				}
			}
			int global_x = rotated_x + position.x;
			int global_y = rotated_y + position.y;
			tiles.push_back(sf::Vector2i(global_x, global_y));
		}
	}
	return tiles;
}