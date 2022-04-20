#pragma once
#include <SFML/Graphics.hpp>
class Button
{

public:
	Button(sf::Vector2i position, sf::Vector2i size);

	void update(sf::Vector2i mousePos, bool lmb);
	bool isClicked();
	bool isActive();
	sf::Sprite getSprite();

	void setText(const std::string& text);

private:

	bool _isClicked;
	bool _isActive;

	sf::Vector2i _position; // top left corner 
	sf::Vector2i _size;

	sf::RenderTexture _texture;

	sf::Font _font;
	std::string _btnText;

	sf::Color _passiveColor;
	sf::Color _activeColor;
};