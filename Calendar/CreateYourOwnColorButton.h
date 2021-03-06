#pragma once
#include <SFML/Graphics.hpp>

class CreateYourOwnColorButton
{
	sf::RectangleShape body;

	sf::Vector2f size;
	sf::Vector2f position;

public:
	CreateYourOwnColorButton(sf::Vector2f position);
	void Draw(sf::RenderWindow& window) { window.draw(body); }
	void Update(sf::RenderWindow& window, sf::Color maincolor);
	bool isHover(sf::RenderWindow& window);

};

