#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

#define GRID_WIDTH 20
#define GRID_HEIGHT 20

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string.h>
#include <array>

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
		std::array<std::array<bool, 4>, 2> grid;
		BlockType(TileType* _tile_type, const std::array<std::array<bool, 4>, 2>& _grid) {
			tile_type = _tile_type;
			grid = _grid;
		}
};

// https://stackoverflow.com/questions/12844475
BlockType BlockType::i(&TileType::white, {{{{0, 0, 0, 0}}, {{1, 1, 1, 1}}}});
BlockType BlockType::j(&TileType::red, {{{{1, 0, 0, 0}}, {{1, 1, 1, 0}}}});
BlockType BlockType::l(&TileType::green, {{{{0, 0, 0, 0}}, {{1, 1, 1, 1}}}});
BlockType BlockType::o(&TileType::blue, {{{{0, 1, 1, 0}}, {{0, 1, 1, 0}}}});
BlockType BlockType::s(&TileType::yellow, {{{{0, 1, 1, 0}}, {{1, 1, 0, 0}}}});
BlockType BlockType::t(&TileType::magenta, {{{{0, 1, 0, 0}}, {{1, 1, 1, 0}}}});
BlockType BlockType::z(&TileType::cyan, {{{{1, 1, 0, 0}}, {{0, 1, 1, 0}}}});
BlockType* BlockType::list[] = {&i, &j, &l, &o, &s, &t, &z};

class Block {
	public:
		BlockType* type;
		sf::Vector2i position;
		Block() {
			type = BlockType::random();
			position = sf::Vector2i(GRID_WIDTH / 2 - 2, 0);
		}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "elnutris");
	window.setFramerateLimit(8);
	
	Block block;

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

		int movement = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			movement--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			movement++;
		bool obstructed = false;
		if (movement != 0) {
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 4; x++) {
					if (!block.type->grid[y][x]) {
						continue;
					}
					int global_x = x + block.position.x;
					int global_y = y + block.position.y;
					if (global_x <= 0 || global_x > GRID_WIDTH || grid[global_y][global_x + movement]) {
						obstructed = true;
						goto after_loop;
					}
				}
			}
		}
		after_loop:
		if (!obstructed) {
			block.position.x += movement;
		}

		shape.setFillColor(block.type->tile_type->color);
		bool landed = false;
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 4; x++) {
				if (!block.type->grid[y][x]) {
					continue;
				}
				int global_x = x + block.position.x;
				int global_y = y + block.position.y;
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
					if (!block.type->grid[y][x]) {
						continue;
					}
					int global_x = x + block.position.x;
					int global_y = y + block.position.y;
					grid[global_y][global_x] = block.type->tile_type;
				}
			}
			landed = false;
			block = Block();
		} else {
			block.position.y++;
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