#include<SFML/Graphics.hpp>
#include<iostream>

#include<ctime>
#include"Player.h"
#include"Map.h"


int main()
{	
	//Map
	Map map("Maps/map3.txt", "Maps/mapSheet.png");
	
	const sf::Vector2u gridSize = map.getGridSize();
	const int cageSize = map.getCageSize();
	const sf::Vector2u mapSize{ gridSize.x * cageSize, gridSize.y * cageSize };

	// Window
	const sf::Vector2u frameSize = { 60, 34 };
	const sf::Vector2u winSize{ frameSize.x * 16, frameSize.y * 16 };
	sf::RenderWindow window(sf::VideoMode(winSize.x, winSize.y), "Game", sf::Style::None);
	window.setFramerateLimit(60);

	// Player
	Player player("Textures/Player32X64.png");
	player.setPos(sf::Vector2f(50.0f, (9 * mapSize.y )/ 11.0f));

	// Variables
	sf::Vector2f offset {0, 0};
	sf::Clock clock;
	int lastDir{ 0 };
	
	// Window loop
	while (window.isOpen())
	{
		const int time = clock.restart().asMilliseconds();

		// Event loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				// X movement
				if (event.key.code == sf::Keyboard::D)
				{
					lastDir = 1;
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					lastDir = -1;
				}
				
				// Jump 
				if ((event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::W) && player.getStatus())
				{
					player.jump();
				}
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				lastDir = 0;
			}
		}


		// Update
		player.move(lastDir);
		player.update(map, time);

		sf::Vector2f playerPos = player.getPos();
		if (playerPos.x > 300)
		{
			offset.x = playerPos.x - 300;
		}
		else
		{
			offset.x = 0;
		}
		
		// Clear
		window.clear(sf::Color::White);
	

		// Draw block
		for (size_t i = 0; i < frameSize.y; i++)
		{
			for (size_t j = 0; j < frameSize.x + 1; j++)
			{ 
				const sf::Vector2f offsetInCages{ offset.x / cageSize, offset.y / cageSize};
				window.draw(map.getSprite(sf::Vector2u(j + offsetInCages.x, i + offsetInCages.y), offset));
			}
		}

		window.draw(player.getSprite(offset));
		window.display();
	}
	system("exit");
}