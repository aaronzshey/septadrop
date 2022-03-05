
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 800

#define GRID_WIDTH 20
#define GRID_HEIGHT 40

class TileType {
	public:
		static TileType white, red, green, blue, yellow, magenta, cyan;
		sf::Color color;
		TileType(sf::Color _color) {
			color = _color;
		}
};

TileType TileType::white = TileType(sf::Color::White);
TileType TileType::red = TileType(sf::Color::Red);
TileType TileType::green = TileType(sf::Color::Green);
TileType TileType::blue = TileType(sf::Color::Blue);
TileType TileType::yellow = TileType(sf::Color::Yellow);
TileType TileType::magenta = TileType(sf::Color::Magenta);
TileType TileType::cyan = TileType(sf::Color::Cyan);

class BlockType {
	public:
		static BlockType i, j, l, o, s, t, z;
		static BlockType* list[];
		static BlockType* random() {
			return list[rand() % 7];
		}
		TileType* tile_type;
		std::vector<std::vector<bool>> grid;
		bool rotate;
		BlockType(TileType* _tile_type, const std::vector<std::vector<bool>> _grid, bool _rotate = true) {
			tile_type = _tile_type;
			grid = _grid;
			rotate = _rotate;
		}
};

// https://gamedev.stackexchange.com/a/17978
BlockType BlockType::i(&TileType::white, {
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
});
BlockType BlockType::j(&TileType::red, {
	{1, 0, 0},
	{1, 1, 1},
	{0, 0, 0}
});
BlockType BlockType::l(&TileType::green, {
	{0, 0, 1},
	{1, 1, 1},
	{0, 0, 0}
});
BlockType BlockType::o(&TileType::blue, {
	{1, 1},
	{1, 1}
}, false);
BlockType BlockType::s(&TileType::yellow, {
	{0, 1, 1},
	{1, 1, 0},
	{0, 0, 0}
});
BlockType BlockType::t(&TileType::magenta, {
	{0, 1, 0},
	{1, 1, 1},
	{0, 0, 0}
});
BlockType BlockType::z(&TileType::cyan, {
	{1, 1, 0},
	{0, 1, 1},
	{0, 0, 0}
});
BlockType* BlockType::list[] = {&i, &j, &l, &o, &s, &t, &z};

class Block {
	public:
		BlockType* type;
		sf::Vector2i position;
		int rotation_state;
		Block() {
			type = BlockType::random();
			position = sf::Vector2i(GRID_WIDTH / 2 - type->grid[0].size() / 2, 0);
			rotation_state = 0;
		}
		std::vector<sf::Vector2i> get_tiles() {
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
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "elnutris");
	
	Block block;

	TileType* grid[GRID_HEIGHT][GRID_WIDTH] = { nullptr };

	int shape_width = WINDOW_WIDTH / GRID_WIDTH;
	int shape_height = WINDOW_HEIGHT / GRID_HEIGHT;
	sf::RectangleShape shape(sf::Vector2f(shape_width, shape_height));

	bool snap, rotate;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Space:
							snap = true;
							break;
						case sf::Keyboard::Up:
							rotate = true;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		window.clear();

		// Fast forward
		window.setFramerateLimit(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ? 8 : 6);

		// Rotation
		if (rotate) {
			block.rotation_state++;
			// Check to see if new rotation state is overlapping any tiles
			for (auto tile : block.get_tiles()) {
				if (tile.x <= 0 || tile.x >= GRID_WIDTH || grid[tile.y][tile.x]) {
					block.rotation_state--;
					break;
				}
			}
			rotate = false;
		}

		// Horizontal movement
		int movement = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			movement--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			movement++;
		bool obstructed = false;
		if (movement != 0) {
			for (auto tile : block.get_tiles()) {
				if (tile.x <= 0 || tile.x >= GRID_WIDTH - 1 || grid[tile.y][tile.x + movement]) {
					obstructed = true;
					goto after_movement_loop;
				}
			}
		}
		after_movement_loop:
		if (!obstructed) {
			block.position.x += movement;
		}

		// Snapping
		int snap_offset = 0;
		while (true) {
			for (auto tile : block.get_tiles()) {
				int y = tile.y + snap_offset;
				if (y == GRID_HEIGHT - 1 || grid[y + 1][tile.x] != nullptr) {
					goto after_snap_loop;
				}
			}
			snap_offset++;
		}
		after_snap_loop:
		if (snap) {
			block.position.y += snap_offset;
			snap = false;
		}

		// Drawing block and land checking
		sf::Color ghost_color = block.type->tile_type->color;
		ghost_color.a = 64;
		bool landed = false;
		for (auto tile : block.get_tiles()) {
			int snap_y = tile.y + snap_offset;
			if (tile.y == GRID_HEIGHT - 1 || grid[tile.y + 1][tile.x] != nullptr) {
				landed = true;
				window.clear();
				break;
			}
			shape.setFillColor(block.type->tile_type->color);
			shape.setPosition(tile.x * shape_width, tile.y * shape_height);
			window.draw(shape);
			shape.setFillColor(ghost_color);
			shape.setPosition(tile.x * shape_width, snap_y * shape_height);
			window.draw(shape);
		}

		// Landing (transfering block to grid and reinitializing)
		if (landed) {
			for (auto tile : block.get_tiles()) {
				grid[tile.y][tile.x] = block.type->tile_type;
			}
			// Check for completed rows
			for (int y = block.position.y; y < block.position.y + block.type->grid.size(); y++) {
				bool completed = true;
				for (int x = 0; x < GRID_WIDTH; x++) {
					if (!grid[y][x]) {
						completed = false;
						break;
					}
				}
				if (!completed) {
					continue;
				}
				for (int z = y - 1; z >= 0; z--) {
					for (int x = 0; x < GRID_WIDTH; x++) {
						grid[z + 1][x] = grid[z][x];
					}
				}
				break;
			}
			block = Block();
		} else {
			block.position.y++;
		}

		// Drawing grid
		for (int y = 0; y < GRID_HEIGHT; y++) {
			for (int x = 0; x < GRID_WIDTH; x++) {
				auto tile_type = grid[y][x];
				if (tile_type == nullptr) {
					// If tile_type is a nullptr (no block), continue
					continue;
				}
				shape.setFillColor(tile_type->color);
				shape.setPosition(x * shape_width, y * shape_height);
				window.draw(shape);
			}
		}		

		window.display();
	}

	return 0;
}