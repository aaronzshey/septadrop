#include <SFML/Graphics.hpp>
#include <sys/types.h>

#include <NumberRenderer.hpp>

NumberRenderer::NumberRenderer(
	sf::Texture _texture,
	sf::IntRect _comma_rect,
	std::initializer_list<sf::IntRect> _numeral_rects
) {
	texture = _texture;
	comma_rect = _comma_rect;
	sprite = sf::Sprite(texture);
	int i = 0;
	for (auto numeral_rect = _numeral_rects.begin(); numeral_rect != _numeral_rects.end(); ++numeral_rect) {
		numeral_rects[i] = *numeral_rect;
		i++;
	}
}

void NumberRenderer::render(sf::RenderWindow* window, uint number, int x, int y) {
	auto number_string = std::to_string(number);
	std::string numeral_string;
	numeral_string.push_back(number_string.back());
	auto numeral_rect = numeral_rects[std::stoi(numeral_string)];
	int x_offset = -numeral_rect.width;
	uint digits = number_string.length();
	for (int i = digits - 1; i >= 0; i--) {
		char numeral_string[] = {number_string[i]};
		auto numeral_rect = numeral_rects[std::stoi(numeral_string)];
		if ((digits - i) % 3 == 1 && i != digits - 1) {
			sprite.setTextureRect(comma_rect);
			sprite.setPosition(x + x_offset, y);
			window->draw(sprite);
			x_offset -= numeral_rect.width;
		}
		sprite.setTextureRect(numeral_rect);
		sprite.setPosition(x + x_offset, y);
		window->draw(sprite);
		if (i == 0) {
			break;
		}
		if ((digits - i) % 3 == 0) {
			x_offset -= comma_rect.width;
			continue;
		}
		numeral_string[0] = number_string[i - 1];
		numeral_rect = numeral_rects[std::stoi(numeral_string)];
		x_offset -= numeral_rect.width;
	}
}