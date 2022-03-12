#include <BlockType.hpp>

BlockType::BlockType(TileType* _tile_type, const std::vector<std::vector<bool>> _grid, bool _rotate) {
	tile_type = _tile_type;
	grid = _grid;
	rotate = _rotate;
	// Used for alignment in "next block" area
	width = 0;
	starting_line = 0;
	for (unsigned int y = 0; y < grid.size(); y++) {
		bool has_content = false;
		for (unsigned int x = 0; x < grid[y].size(); x++) {
			if (grid[y][x]) {
				width = std::max({width, x + 1});
				has_content = true;
			}
		}
		if (has_content) {
			if (height == 0) {
				starting_line = y;
			}
			height = y + 1 - starting_line;
		}
	}
}

BlockType* BlockType::random() {
	init();
	return &list[rand() % list.size()];
}

void BlockType::init() {
	if (inited) {
		return;
	}

	// I block
	list.push_back(BlockType(new TileType(
			sf::IntRect(0,             0, TILE_SIZE, TILE_SIZE),
			sf::IntRect(0,             TILE_SIZE, TILE_SIZE, TILE_SIZE)
		), {
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		}
	));

	// J Block
	list.push_back(BlockType(new TileType(
			sf::IntRect(TILE_SIZE,     0, TILE_SIZE, TILE_SIZE),
			sf::IntRect(TILE_SIZE,     TILE_SIZE, TILE_SIZE, TILE_SIZE)
		), {
			{1, 0, 0},
			{1, 1, 1},
			{0, 0, 0}
		}
	));

	// L Block
	list.push_back(BlockType(new TileType(
			sf::IntRect(TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE),
			sf::IntRect(TILE_SIZE * 2, TILE_SIZE, TILE_SIZE, TILE_SIZE)
		), {
			{0, 0, 1},
			{1, 1, 1},
			{0, 0, 0}
		}
	));

	// O Block
	list.push_back(BlockType(new TileType(
			sf::IntRect(TILE_SIZE * 3, 0, TILE_SIZE, TILE_SIZE),
			sf::IntRect(TILE_SIZE * 3, TILE_SIZE, TILE_SIZE, TILE_SIZE)
		), {
			{1, 1},
			{1, 1}
		}, false
	));

	// S Block
	list.push_back(BlockType(new TileType(
			sf::IntRect(TILE_SIZE * 4, 0, TILE_SIZE, TILE_SIZE),
			sf::IntRect(TILE_SIZE * 4, TILE_SIZE, TILE_SIZE, TILE_SIZE)
		), {
			{0, 1, 1},
			{1, 1, 0},
			{0, 0, 0}
		}
	));

	// T Block
	list.push_back(BlockType(new TileType(
			sf::IntRect(TILE_SIZE * 5, 0, TILE_SIZE, TILE_SIZE),
			sf::IntRect(TILE_SIZE * 5, TILE_SIZE, TILE_SIZE, TILE_SIZE)
		), {
			{0, 1, 0},
			{1, 1, 1},
			{0, 0, 0}
		}
	));

	// Z Block
	list.push_back(BlockType(new TileType(
			sf::IntRect(TILE_SIZE * 6, 0, TILE_SIZE, TILE_SIZE),
			sf::IntRect(TILE_SIZE * 6, TILE_SIZE, TILE_SIZE, TILE_SIZE)
		), {
			{1, 1, 0},
			{0, 1, 1},
			{0, 0, 0}
		}
	));

	inited = true;
}