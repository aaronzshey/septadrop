#pragma once

#include <SFML/Graphics.hpp>

class NumberRenderer {
	public:
		sf::Texture texture;
		sf::IntRect comma_rect;
		sf::IntRect numeral_rects[10];
		NumberRenderer(
			sf::Texture _texture,
			sf::IntRect _comma_rect,
			std::initializer_list<sf::IntRect> _numeral_rects
		);
		void render(sf::RenderWindow* window, unsigned int number, int x, int y);
	private:
		sf::Sprite sprite;
};