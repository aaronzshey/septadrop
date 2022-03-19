use crate::structs::TileType;
use crate::config::TILE_SIZE;
use sfml::graphics::IntRect;

pub struct BlockType {
	pub tile_type: TileType,
	pub grid: Vec<Vec<bool>>,
	pub width: u32,
	pub height: u32,
	pub starting_line: u32,
	pub rotate: bool
}

impl BlockType {
	pub fn new(tile_type: TileType, grid: Vec<Vec<bool>>, rotate: bool) -> Self {
		let mut width: u32 = 0;
		let mut height: u32 = 0;
		let mut starting_line: u32 = 0;
		for (y, row) in grid.iter().enumerate() {
			let mut has_content = false;
			for (x, cell) in row.iter().enumerate() {
				if *cell {
					width = std::cmp::max(width, x as u32 + 1);
					has_content = true;
				}
			}
			if has_content {
				if height == 0 {
					starting_line = y as u32;
				}
				height = y as u32 + 1 - starting_line;
			}
		}
		Self {
			tile_type,
			grid,
			width,
			height,
			starting_line,
			rotate
		}
	}

	pub fn init_list() -> Vec<Self> {
		let mut list = Vec::new();
		let tile_size = TILE_SIZE as i32;

		const Y: bool = true;
		const N: bool = false;

		// I block
		list.push(Self::new(
			TileType::new(
				IntRect::new(0, 0, tile_size, tile_size),
				IntRect::new(0, tile_size, tile_size, tile_size)
			),
			vec![
				vec![N, N, N, N],
				vec![Y, Y, Y, Y],
				vec![N, N, N, N],
				vec![N, N, N, N]
			],
			true
		));

		// J Block
		list.push(Self::new(
			TileType::new(
				IntRect::new(tile_size, 0, tile_size, tile_size),
				IntRect::new(tile_size, tile_size, tile_size, tile_size),
			),
			vec![
				vec![Y, N, N],
				vec![Y, Y, Y],
				vec![N, N, N]
			],
			true
		));

		// L Block
		list.push(Self::new(
			TileType::new(
				IntRect::new(tile_size * 2, 0, tile_size, tile_size),
				IntRect::new(tile_size * 2, tile_size, tile_size, tile_size),
			),
			vec![
				vec![N, N, Y],
				vec![Y, Y, Y],
				vec![N, N, N]
			],
			true
		));

		// O Block
		list.push(Self::new(
			TileType::new(
				IntRect::new(tile_size * 3, 0, tile_size, tile_size),
				IntRect::new(tile_size * 3, tile_size, tile_size, tile_size),
			),
			vec![
				vec![Y, Y],
				vec![Y, Y]
			],
			false
		));

		// S Block
		list.push(Self::new(
			TileType::new(
				IntRect::new(tile_size * 4, 0, tile_size, tile_size),
				IntRect::new(tile_size * 4, tile_size, tile_size, tile_size),
			),
			vec![
				vec![N, Y, Y],
				vec![Y, Y, N],
				vec![N, N, N]
			],
			true
		));

		// T Block
		list.push(Self::new(
			TileType::new(
				IntRect::new(tile_size * 5, 0, tile_size, tile_size),
				IntRect::new(tile_size * 5, tile_size, tile_size, tile_size),
			),
			vec![
				vec![N, Y, N],
				vec![Y, Y, Y],
				vec![N, N, N]
			],
			true
		));

		// Z Block
		list.push(Self::new(
			TileType::new(
				IntRect::new(tile_size * 6, 0, tile_size, tile_size),
				IntRect::new(tile_size * 6, tile_size, tile_size, tile_size)
			),
			vec![
				vec![Y, Y, N],
				vec![N, Y, Y],
				vec![N, N, N]
			],
			true
		));
		
		list
	}
}