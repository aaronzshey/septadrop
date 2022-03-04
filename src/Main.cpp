#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string.h>
#include <array>

class TileType {
	public:
		sf::Color color;
		TileType(sf::Color _color) {
			color = _color;
		}
};

class BlockType {
	public:
		TileType* tile_type;
		std::array<std::array<bool, 4>, 2> grid;
		BlockType(TileType* _tile_type, const std::array<std::array<bool, 4>, 2>& _grid) {
			tile_type = _tile_type;
			grid = _grid;
		}
};

int main()
{
	#define WINDOW_WIDTH 400
	#define WINDOW_HEIGHT 400
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "elnutris");
	window.setFramerateLimit(8);

	TileType white(sf::Color::White);
	TileType red(sf::Color::Red);
	TileType green(sf::Color::Green);
	TileType blue(sf::Color::Blue);
	TileType yellow(sf::Color::Yellow);
	TileType magenta(sf::Color::Magenta);
	TileType cyan(sf::Color::Cyan);

	// https://stackoverflow.com/questions/12844475
	BlockType i(&white, {{{{0, 0, 0, 0}}, {{1, 1, 1, 1}}}});
	BlockType j(&red, {{{{1, 0, 0, 0}}, {{1, 1, 1, 0}}}});
	BlockType l(&green, {{{{0, 0, 0, 0}}, {{1, 1, 1, 1}}}});
	BlockType o(&blue, {{{{0, 1, 1, 0}}, {{0, 1, 1, 0}}}});
	BlockType s(&yellow, {{{{0, 1, 1, 0}}, {{1, 1, 0, 0}}}});
	BlockType t(&magenta, {{{{0, 1, 0, 0}}, {{1, 1, 1, 0}}}});
	BlockType z(&cyan, {{{{1, 1, 0, 0}}, {{0, 1, 1, 0}}}});
	BlockType block_types[] = {i, j, l, o, s, t, z};

	#define GRID_WIDTH 20
	#define GRID_HEIGHT 20
	
	auto current_block = &block_types[rand() % 7];;
	auto current_block_position = sf::Vector2i(GRID_WIDTH / 2 - 2, 0);

	TileType* grid[GRID_HEIGHT][GRID_WIDTH] = { nullptr };

	int shape_width = WINDOW_WIDTH / GRID_WIDTH;
	int shape_height = WINDOW_HEIGHT / GRID_HEIGHT;
	sf::RectangleShape shape(sf::Vector2f(shape_width, shape_height));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			current_block_position.x--;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			current_block_position.x++;
		}

		shape.setFillColor(current_block->tile_type->color);
		bool landed = false;
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 4; x++) {
				if (!current_block->grid[y][x]) {
					continue;
				}
				int global_x = x + current_block_position.x;
				int global_y = y + current_block_position.y;
				if (global_y == GRID_HEIGHT - 1 || grid[global_y + 1][global_x] != nullptr) {
					landed = true;
				}
				shape.setPosition(global_x * shape_width, global_y * shape_height);
				window.draw(shape);
			}
		}
		if (landed) {
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 4; x++) {
					if (!current_block->grid[y][x]) {
						continue;
					}
					int global_x = x + current_block_position.x;
					int global_y = y + current_block_position.y;
					grid[global_y][global_x] = current_block->tile_type;
				}
			}
			landed = false;
			current_block = &block_types[rand() % 7];
			current_block_position = sf::Vector2i(GRID_WIDTH / 2 - 2, 0);
		} else {
			current_block_position.y++;
		}

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