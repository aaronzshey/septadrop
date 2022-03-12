#pragma once

#include "TileType.hpp"
#include <vector>

class BlockType {
	public:
		TileType* tile_type;
		std::vector<std::vector<bool>> grid;
		unsigned int  width, height, starting_line;
		bool rotate;
		BlockType(TileType* _tile_type, const std::vector<std::vector<bool>> _grid, bool _rotate = true);

		static BlockType* random();
		static void init();
	private:
		inline static std::vector<BlockType> list = {};
		inline static bool inited = false;
};