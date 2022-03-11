#pragma once

#include <TileType.hpp>

class BlockType {
	public:
		static BlockType i, j, l, o, s, t, z;
		static BlockType* list[];
		static BlockType* random() {
			return list[rand() % 7];
		}
		TileType* tile_type;
		std::vector<std::vector<bool>> grid;
		uint width, height, starting_line;
		bool rotate;
		BlockType(TileType* _tile_type, const std::vector<std::vector<bool>> _grid, bool _rotate = true);
};