
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <iterator>
#include <string>

#define TILE_SIZE 20

#define GRID_WIDTH 14
#define GRID_HEIGHT 20

#define WINDOW_WIDTH GRID_WIDTH * TILE_SIZE
#define WINDOW_HEIGHT GRID_HEIGHT * TILE_SIZE

class TileType {
	public:
		sf::IntRect texture_rect;
		sf::IntRect ghost_texture_rect;
		TileType(sf::IntRect _texture_rect, sf::IntRect _ghost_texture_rect) {
			texture_rect = _texture_rect;
			ghost_texture_rect = _ghost_texture_rect;
		}
};

TileType tile_type_0(
	sf::IntRect(0,             0, TILE_SIZE, TILE_SIZE),
	sf::IntRect(0,             TILE_SIZE, TILE_SIZE, TILE_SIZE)
);
TileType tile_type_1(
	sf::IntRect(TILE_SIZE,     0, TILE_SIZE, TILE_SIZE),
	sf::IntRect(TILE_SIZE,     TILE_SIZE, TILE_SIZE, TILE_SIZE)
);
TileType tile_type_2(
	sf::IntRect(TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE),
	sf::IntRect(TILE_SIZE * 2, TILE_SIZE, TILE_SIZE, TILE_SIZE)
);
TileType tile_type_3(
	sf::IntRect(TILE_SIZE * 3, 0, TILE_SIZE, TILE_SIZE),
	sf::IntRect(TILE_SIZE * 3, TILE_SIZE, TILE_SIZE, TILE_SIZE)
);
TileType tile_type_4(
	sf::IntRect(TILE_SIZE * 4, 0, TILE_SIZE, TILE_SIZE),
	sf::IntRect(TILE_SIZE * 4, TILE_SIZE, TILE_SIZE, TILE_SIZE)
);
TileType tile_type_5(
	sf::IntRect(TILE_SIZE * 5, 0, TILE_SIZE, TILE_SIZE),
	sf::IntRect(TILE_SIZE * 5, TILE_SIZE, TILE_SIZE, TILE_SIZE)
);
TileType tile_type_6(
	sf::IntRect(TILE_SIZE * 6, 0, TILE_SIZE, TILE_SIZE),
	sf::IntRect(TILE_SIZE * 6, TILE_SIZE, TILE_SIZE, TILE_SIZE)
);

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
BlockType BlockType::i(&tile_type_0, {
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
});
BlockType BlockType::j(&tile_type_1, {
	{1, 0, 0},
	{1, 1, 1},
	{0, 0, 0}
});
BlockType BlockType::l(&tile_type_2, {
	{0, 0, 1},
	{1, 1, 1},
	{0, 0, 0}
});
BlockType BlockType::o(&tile_type_3, {
	{1, 1},
	{1, 1}
}, false);
BlockType BlockType::s(&tile_type_4, {
	{0, 1, 1},
	{1, 1, 0},
	{0, 0, 0}
});
BlockType BlockType::t(&tile_type_5, {
	{0, 1, 0},
	{1, 1, 1},
	{0, 0, 0}
});
BlockType BlockType::z(&tile_type_6, {
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
	srand(time(NULL));
	
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "elnutris");
	window.setFramerateLimit(60);
	
	Block block;

	TileType* grid[GRID_HEIGHT][GRID_WIDTH] = { nullptr };

	sf::Texture texture;
	texture.loadFromFile("../res/texture.png");
	sf::Sprite sprite;
	sprite.setTexture(texture);

	bool snap, rotate, move_left, move_right;
	sf::Clock update_clock;
	sf::Clock move_clock;

	int score = 0;

	sf::Font font;
	font.loadFromFile("../res/font.ttf");

	sf::Text text;
	text.setFont(font);
	text.setString("0");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(8, 0);

	int update_interval = 250;

	auto clear_color = sf::Color(73, 52, 61);

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
						case sf::Keyboard::Left:
							move_left = true;
							break;
						case sf::Keyboard::Right:
							move_right = true;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		bool is_update_frame = update_clock.getElapsedTime().asMilliseconds() > (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? update_interval / 2 : update_interval);
		if (is_update_frame) {
			update_clock.restart();
		}

		bool is_move_frame = move_clock.getElapsedTime().asMilliseconds() > update_interval / 2;
		if (is_move_frame) {
			move_clock.restart();
		}

		// Rotation
		if (rotate && is_move_frame) {
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
		if (is_move_frame) {
			int movement = 0;
			if (move_left || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
				movement--;
				move_left = false;
			}
			if (move_right ||sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
				movement++;
				move_right = false;
			}
			if (movement != 0) {
				for (auto tile : block.get_tiles()) {
					if (tile.x + movement < 0 || tile.x + movement >= GRID_WIDTH || grid[tile.y][tile.x + movement]) {
						goto after_movement_loop;
					}
				}
				block.position.x += movement;
			}
		}
		after_movement_loop:

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

		// Land checking
		bool landed = false;
		for (auto tile : block.get_tiles()) {
			if (tile.y == GRID_HEIGHT - 1 || grid[tile.y + 1][tile.x] != nullptr) {
				landed = true;
				break;
			}
		}

		// Draw block
		window.clear(clear_color);
		if (!landed) {
			for (auto tile : block.get_tiles()) {
				int snap_y = tile.y + snap_offset;
				sprite.setTextureRect(block.type->tile_type->texture_rect);
				sprite.setPosition(tile.x * TILE_SIZE, tile.y * TILE_SIZE);
				window.draw(sprite);
				sprite.setTextureRect(block.type->tile_type->ghost_texture_rect);
				sprite.setPosition(tile.x * TILE_SIZE, snap_y * TILE_SIZE);
				window.draw(sprite);
			}
		}

		// Landing (transfering block to grid and reinitializing)
		if (landed) {
			if (block.position.y == 0) {
				update_interval += score * 10;
				score = 0;
				text.setString("0");
				for (int y = 0; y < GRID_HEIGHT; y++) {
					for (int x = 0; x < GRID_WIDTH; x++) {
						grid[y][x] = nullptr;
					}
				}
			} else {
				for (auto tile : block.get_tiles()) {
					grid[tile.y][tile.x] = block.type->tile_type;
				}
				// Check for completed rows
				for (int y = 0; y < GRID_HEIGHT; y++) {
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
					score++;
					update_interval -= 10;
					text.setString(std::to_string(score));
				}
			}
			block = Block();
		} else if(is_update_frame) {
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
				sprite.setTextureRect(tile_type->texture_rect);
				sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
				window.draw(sprite);
			}
		}		
		window.draw(text);

		window.display();
	}

	return 0;
}