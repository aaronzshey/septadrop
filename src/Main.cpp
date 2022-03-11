#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <initializer_list>
#include <iterator>
#include <math.h>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>

#include <packed/SharedResources.hpp>

#include <Config.hpp>
#include <NumberRenderer.hpp>
#include <TileType.hpp>
#include <BlockType.hpp>
#include <Block.hpp>

uint get_level(int lines) {
	return std::min(lines / LINES_PER_LEVEL, 15);
}

uint get_update_interval(int level) {
	// From Tetris Worlds, see https://harddrop.com/wiki/Tetris_Worlds#Gravity
	return pow(0.8 - (level - 1) * 0.007, level - 1) * 1000;
}

int main()
{
	srand(time(NULL));
	
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "elnutris", sf::Style::Close);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false); // prevent keys from retriggering when held
	
	Block block;
	Block next_block;

	TileType* grid[GRID_HEIGHT][GRID_WIDTH] = { nullptr };

	sf::Texture blocks_texture;
	blocks_texture.loadFromMemory(&BLOCKS_TEXTURE_DATA, sizeof(BLOCKS_TEXTURE_DATA));
	sf::Sprite sprite;
	sprite.setTexture(blocks_texture);

	sf::Texture background_texture;
	background_texture.loadFromMemory(&BACKGROUND_TEXTURE_DATA, sizeof(BACKGROUND_TEXTURE_DATA));
	sf::Sprite background;
	background.setTexture(background_texture);

	sf::Texture numeral_texture;
	numeral_texture.loadFromMemory(&NUMERALS_TEXTURE_DATA, sizeof(NUMERALS_TEXTURE_DATA));
	NumberRenderer number_renderer(numeral_texture, sf::IntRect(134, 0, 10, 16), {
		sf::IntRect(0, 0, 14, 16),
		sf::IntRect(14, 0, 8, 16),
		sf::IntRect(22, 0, 14, 16),
		sf::IntRect(36, 0, 14, 16),
		sf::IntRect(50, 0, 14, 16),
		sf::IntRect(64, 0, 14, 16),
		sf::IntRect(78, 0, 14, 16),
		sf::IntRect(92, 0, 14, 16),
		sf::IntRect(106, 0, 14, 16),
		sf::IntRect(120, 0, 14, 16)
	});

	bool rotate = false;
	bool move_left = false;
	bool move_right = false;
	bool move_left_immediate = false;
	bool move_right_immediate = false;
	bool snap = false;
	sf::Clock update_clock;
	sf::Clock move_clock;

	// https://stackoverflow.com/a/478088
	const char *homedir;
	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}

	std::string highscore_file_path = homedir;
	highscore_file_path += "/.elnutris";

	if (!std::filesystem::exists(highscore_file_path)) {
		std::ofstream highscore_file(highscore_file_path);
		highscore_file << "0";
		highscore_file.close();
	}
	std::fstream highscore_file(highscore_file_path);
	std::string highscore_string;
	highscore_file >> highscore_string;
	uint highscore = std::stoi(highscore_string);

	uint score = 0;
	uint lines = 0;
	uint blocks = 0;
	uint tiles = 0;

	uint update_interval = get_update_interval(0);

	// https://sfxr.me/#57uBnWWZeyDTsBRrJsAp2Vwd76cMVrdeRQ7DirNQW5XekKxcrCUNx47Zggh7Uqw4R5FdeUpyk362uhjWmpNHmqxE7JBp3EkxDxfJ1VjzMRpuSHieW6B5iyVFM
	sf::SoundBuffer rotate_buffer;
	rotate_buffer.loadFromMemory(&ROTATE_AUDIO_DATA, sizeof(ROTATE_AUDIO_DATA));

	sf::Sound rotate_sound;
	rotate_sound.setBuffer(rotate_buffer);

	// https://sfxr.me/#57uBnWTMa2LUtaPa3P8xWZekiRxNwCPFWpRoPDVXDJM9KHkiGJcs6J62FRcjMY5oVNdT73MtmUf5rXCPvSZWL7AZuTRWWjKbPKTpZjT85AcZ6htUqTswkjksZ
	sf::SoundBuffer snap_buffer;
	snap_buffer.loadFromMemory(&SNAP_AUDIO_DATA, sizeof(SNAP_AUDIO_DATA));

	sf::Sound snap_sound;
	snap_sound.setBuffer(snap_buffer);

	// https://sfxr.me/#57uBnWbareN7MJJsWGD8eFCrqjikS9f8JXg8jvmKzMdVtqmRsb81eToSUpnkqgFhvxD2QoAjpw4SmGZHZjbhEiPQKetRSHCHXYFZzD7Q6RVVS9CRSeRAb6bZp
	sf::SoundBuffer game_over_buffer;
	game_over_buffer.loadFromMemory(&GAME_OVER_AUDIO_DATA, sizeof(GAME_OVER_AUDIO_DATA));

	sf::Sound game_over_sound;
	game_over_sound.setBuffer(game_over_buffer);

	// https://sfxr.me/#7BMHBGMfGk8EHV8czJkUucUm8EMAnMNxiqYyTfKkMpHFJu44GEdD7xP6E8NM3K7RKRExTpagPBAiWf7BLtC52CEWJVGHh8hwDLygoEG86tcPth2UtmfdrXLoh
	sf::SoundBuffer row_clear_buffer;
	row_clear_buffer.loadFromMemory(&ROW_CLEAR_AUDIO_DATA, sizeof(ROW_CLEAR_AUDIO_DATA));

	sf::Sound row_clear_sound;
	row_clear_sound.setBuffer(row_clear_buffer);

	// https://sfxr.me/#57uBnWg8448kTPqWAxeDvZ5CP5JWbrfJGWuRcTjva5uX3vvBnEAZ6SfiH9oLKMXgsusuJwGWx6KPfvLfHtqnhLxr476ptGv4jPbfNhQaFMYeMHFdHk9SotQ4X
	sf::SoundBuffer level_up_buffer;
	level_up_buffer.loadFromMemory(&LEVEL_UP_AUDIO_DATA, sizeof(LEVEL_UP_AUDIO_DATA));

	sf::Sound level_up_sound;
	level_up_sound.setBuffer(level_up_buffer);

	// https://sfxr.me/#34T6PkzvrkfdahGDBAh1uYGXTwZ8rG54kxfHpgdVCPxqG7yyK5UuqgiK9Z8Q5177itxbkSNfLSHm4zTkemT4iyxJpW89VJx82feaq8qxZeA5AJR2nWZZR59hq
	sf::SoundBuffer new_highscore_buffer;
	new_highscore_buffer.loadFromMemory(&NEW_HIGHSCORE_AUDIO_DATA, sizeof(NEW_HIGHSCORE_AUDIO_DATA));

	sf::Sound new_highscore_sound;
	new_highscore_sound.setBuffer(new_highscore_buffer);

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
							move_left_immediate = true;
							move_clock.restart();
							break;
						case sf::Keyboard::Right:
							move_right = true;
							move_right_immediate = true;
							move_clock.restart();
							break;
						default:
							break;
					}
					break;
				case sf::Event::KeyReleased:
					switch (event.key.code) {
						case sf::Keyboard::Left:
							move_left = false;
							break;
						case sf::Keyboard::Right:
							move_right = false;
							break;
						default:
							break;
					}
				default:
					break;
			}
		}

		bool is_update_frame = update_clock.getElapsedTime().asMilliseconds() > (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? std::min({update_interval, 125u}) : update_interval);
		if (is_update_frame) {
			update_clock.restart();
		}

		bool is_move_frame = move_clock.getElapsedTime().asMilliseconds() > 125;
		if (is_move_frame) {
			move_clock.restart();
		}

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
			rotate_sound.play();
		}

		// Horizontal movement
		int movement = 0;
		if (move_left_immediate || is_move_frame && move_left) {
			movement--;
			move_left_immediate = false;
		}
		if (move_right_immediate || is_move_frame && move_right) {
			movement++;
			move_right_immediate = false;
		}
		if (movement != 0) {
			for (auto tile : block.get_tiles()) {
				if (tile.x + movement < 0 || tile.x + movement >= GRID_WIDTH || grid[tile.y][tile.x + movement]) {
					goto after_movement_loop;
				}
			}
			block.position.x += movement;
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
		bool landed = snap;
		if (snap) {
			block.position.y += snap_offset;
			snap = false;
			snap_sound.play();
		}

		// Land checking
		if (!snap && is_update_frame) {
			for (auto tile : block.get_tiles()) {
				if (tile.y == GRID_HEIGHT - 1 || grid[tile.y + 1][tile.x] != nullptr) {
					landed = true;
					break;
				}
			}
		}

		// Clear window
		// Normally, one would run window.clear(),
		// but the background image covers the entire window.
		window.draw(background);
		
		// Draw block
		if (!landed) {
			for (auto tile : block.get_tiles()) {
				int snap_y = tile.y + snap_offset;
				sprite.setTextureRect(block.type->tile_type->texture_rect);
				sprite.setPosition(PLAYFIELD_X + tile.x * TILE_SIZE, PLAYFIELD_Y + tile.y * TILE_SIZE);
				window.draw(sprite);
				sprite.setTextureRect(block.type->tile_type->ghost_texture_rect);
				sprite.setPosition(PLAYFIELD_X + tile.x * TILE_SIZE, PLAYFIELD_Y + snap_y * TILE_SIZE);
				window.draw(sprite);
			}
		}

		// Draw next block
		auto next_block_tiles = next_block.get_tiles();
		// This is assuming the next block spawns unrotated.
		// Refactoring is needed if random rotations are added
		uint x_offset = next_block.type->width * TILE_SIZE / 2;
		uint y_offset = (next_block.type->height + next_block.type->starting_line * 2) * TILE_SIZE / 2;
		for (auto tile : next_block_tiles) {
			sprite.setTextureRect(next_block.type->tile_type->texture_rect);
			sprite.setPosition(
				370 + (tile.x - next_block.position.x) * TILE_SIZE - x_offset,
				70 + (tile.y - next_block.position.y) * TILE_SIZE - y_offset
			);
			window.draw(sprite);
		}

		// Landing (transfering block to grid and reinitializing)
		if (landed) {
			if (block.position.y == 0) {
				score = 0;
				lines = 0;
				blocks = 0;
				tiles = 0;
				for (int y = 0; y < GRID_HEIGHT; y++) {
					for (int x = 0; x < GRID_WIDTH; x++) {
						grid[y][x] = nullptr;
					}
				}
				game_over_sound.play();
			} else {
				tiles += block.get_tiles().size();
				blocks++;
				for (auto tile : block.get_tiles()) {
					grid[tile.y][tile.x] = block.type->tile_type;
				}
				uint cleared_lines = 0;
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
					cleared_lines++;
				}
				uint scored;
				switch (cleared_lines) {
					case 0:
						scored = 0;
						break;
					case 1:
						scored = POINTS_1_LINE;
						break;
					case 2:
						scored = POINTS_2_LINES;
						break;
					case 3:
						scored = POINTS_3_LINES;
						break;
					default:
						scored = POINTS_4_LINES;
						break;
				}
				if (scored > 0) {
					int level = get_level(lines);
					scored *= level + 1;
					if (score + scored > highscore && score < highscore) {
						new_highscore_sound.play();
					}
					score += scored;
					lines += cleared_lines;
					if (level != get_level(lines)) {
						level_up_sound.play();
					}
					if (score > highscore) {
						highscore = score;
						std::ofstream highscore_file(highscore_file_path);
						highscore_file << highscore;
						highscore_file.close();
					}
					update_interval = get_update_interval(level);
					row_clear_sound.play();
				}
			}
			block = next_block;
			next_block = Block();
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
				sprite.setPosition(PLAYFIELD_X + x * TILE_SIZE, PLAYFIELD_Y + y * TILE_SIZE);
				window.draw(sprite);
			}
		}

		number_renderer.render(&window, score, 477, 162);
		number_renderer.render(&window, highscore, 477, 202);
		number_renderer.render(&window, lines, 477, 242);
		number_renderer.render(&window, get_level(lines), 477, 282);
		number_renderer.render(&window, blocks, 477, 322);
		number_renderer.render(&window, tiles, 477, 362);

		window.display();
	}

	return 0;
}