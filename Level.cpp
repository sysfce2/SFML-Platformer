#include "Level.h"

Level::Level(const std::string& mapPath, const std::string& mapSheetPath, const std::string& playerTexturePath ,sf::Vector2u winPixelSize)
{
	texture.create(winPixelSize.x, winPixelSize.y);

	map = new Map(mapPath, mapSheetPath);
	tileSize = map->getCageSize();
	gridTileSize = map->getGridSize();
	mapPixelSize = { gridTileSize.x * tileSize, gridTileSize.y * tileSize };
	winTileSize = { winPixelSize.x / tileSize, winPixelSize.y / tileSize + 1u };

	player = new Player(playerTexturePath);
	player->setPos(sf::Vector2f(10 * tileSize, 20 * tileSize)); // todo initial position of player
	controls.walkDirection = 0;
	controls.grabDirection = 0;

	cam = new Camera(winPixelSize, mapPixelSize);
	offset = { 0, 0 };
}

int Level::update(const std::vector<sf::Event>& events)
{
	poolControls(events);

	//std::cout << controls.walkDirection << "\n";

	player->move(controls.walkDirection);

	if (player->isStairsAvailable() && controls.grabDirection != 0)
	{
		player->grab(controls.grabDirection);
	}

	if (controls.isJump)
	{
		player->jump();
	}

	player->update(*map, clock.restart().asMilliseconds());

	return 0; // nothing to do
}

sf::Sprite Level::getSprite()
{
	// clear
	texture.clear(sf::Color::White);

	// generation offsets
	offset = cam->calculateOffsets(player->getPos(), player->getVel());

	// map draw
	const sf::Vector2i offsetInCages{ static_cast<int> (offset.x / tileSize), static_cast<int> (offset.y / tileSize) };
	for (float i = 0; i < winTileSize.y + 1; i++)
	{
		for (float j = 0; j < winTileSize.x + 1; j++)
		{
			sf::Sprite sprt = map->getSprite(sf::Vector2f(j + offsetInCages.x, i + offsetInCages.y), offset);
			texture.draw(sprt);
		}
	}

	// player draw
	texture.draw(player->getSprite(offset));

	texture.display();
	return sf::Sprite(texture.getTexture());
}

void Level::poolControls(const std::vector<sf::Event>& events)
{
	controls.isJump = false;

	for(const auto event : events)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			// X movement
			if (event.key.code == sf::Keyboard::D)
			{
				controls.walkDirection = 1;
			}
			else if (event.key.code == sf::Keyboard::A)
			{
				controls.walkDirection = -1;
			}

			// Jump 
			if (event.key.code == sf::Keyboard::Space && (player->isOnGround() || player->isOnStairs()))
			{
				controls.isJump = true;
			}

			// Stairs grab
			if (player->isStairsAvailable())
			{
				if (event.key.code == sf::Keyboard::W)
				{
					controls.grabDirection = -1;
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					controls.grabDirection = 1;
				}
			}
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			controls.walkDirection = 0;
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			controls.grabDirection = 0;
		}
	}
}
