#include <cstdlib>

#include <BlockType.hpp>
#include <TileType.hpp>

BlockType::BlockType(TileType* _tile_type, const std::vector<std::vector<bool>> _grid, bool _rotate) {
	tile_type = _tile_type;
	grid = _grid;
	rotate = _rotate;
	// Used for alignment in "next block" area
	width = 0;
	starting_line = 0;
	for (uint y = 0; y < grid.size(); y++) {
		bool has_content = false;
		for (uint x = 0; x < grid[y].size(); x++) {
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