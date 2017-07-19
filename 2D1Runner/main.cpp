#include <iostream>
#include <SFML/Graphics.hpp>

#define W_W 800
#define W_H 600
#define FLOOR_HEIGHT 200
#define FLOOR_Y W_H - FLOOR_HEIGHT

#define RATIO 100

#define PLAYER_H 30
#define PLAYER_OFFSET 200

#define ENERGY_MAX 100
#define ENERGY_DRAIN 20
#define ENERGY_REGEN 10
#define ENERGY_BAR_W 150

#define MAX_FENCES 10
#define MIN_BETWEEN_F 100
#define MAX_BETWEEN_F 500
#define SCORE_FENCE 1

#define MAX_WALLS 10
#define MIN_BETWEEN_W 50
#define MAX_BETWEEN_W 1500
#define SCORE_WALL 2

int main()
{
	// Window && view
    sf::RenderWindow window(sf::VideoMode(W_W, W_H), "2+1D Runner");
	sf::View camera(sf::FloatRect(0, 0, W_W, W_H));
	sf::View hudView(sf::FloatRect(0, 0, W_W, W_H));

	// floor
	sf::RectangleShape floor(sf::Vector2f(W_W, FLOOR_HEIGHT));
	floor.setPosition(0, FLOOR_Y);
	floor.setFillColor(sf::Color::Black);

	// score
	bool hud = true;
	sf::Font font;
	if (!font.loadFromFile("Roboto-Regular.ttf"))
	{
		hud = false;
		std::cerr << "Err : Unable to load font. No HUD info available" << std::endl;
	}
	int score = 0;
	sf::Text scoreText;
	if(hud)
	{
		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setColor(sf::Color::Black);
		scoreText.setPosition(10, 10);
		
		scoreText.setString(std::to_string(score));
	}
	
	// player
	sf::RectangleShape player(sf::Vector2f(PLAYER_H, PLAYER_H));
	player.setFillColor(sf::Color::Blue);
	
	sf::Vector2f p_pos(PLAYER_OFFSET, FLOOR_Y - PLAYER_H * 3);
	sf::Vector2f p_speed(200.f, 0.f);
	sf::Vector2f g(3.f, 9.8f * RATIO);
	bool p_jump = false;

	float p_energy = ENERGY_MAX;
	bool p_phase = false;
	sf::RectangleShape energyBar(sf::Vector2f(ENERGY_BAR_W, 15));
	energyBar.setPosition(10, 40);
	energyBar.setFillColor(sf::Color::Green);

	//obstables
	std::vector<float> fences;
	float lastScoredFence = 0;
	fences.push_back(W_W);
	sf::RectangleShape fence(sf::Vector2f(PLAYER_H / 3, PLAYER_H));
	fence.setFillColor(sf::Color::Red);

	std::vector<float> walls;
	float lastScoredWall = 0;
	walls.push_back(W_W);
	sf::RectangleShape wall(sf::Vector2f(PLAYER_H, PLAYER_H * 5));
	wall.setFillColor(sf::Color::Black);

	/* game loop */
	sf::Clock clock;
	srand(time(NULL));
    while (window.isOpen())
    {
		/*read events*/
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if(event.type == sf::Event::KeyReleased)
			{
				if(event.key.code == sf::Keyboard::Escape)
					window.close();
				else if(event.key.code == sf::Keyboard::Z)
					p_phase = false;
			}
			if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Space)
				{
					if(!p_jump)
					{
						p_jump = true;
						p_speed.y = -4 * RATIO;
					}
				}
				else if(event.key.code == sf::Keyboard::Z)
					p_phase = true;
			}
        }

		/* update game logic */
		sf::Time elapsed = clock.restart();

		// update player speed and position
		p_speed.x += g.x * elapsed.asSeconds();
		p_speed.y += g.y * elapsed.asSeconds();
		p_pos.y += p_speed.y * elapsed.asSeconds();
		p_pos.x += p_speed.x * elapsed.asSeconds();
		// try collision with floor
		if(p_pos.y > FLOOR_Y - PLAYER_H)
		{
			p_jump = false;
			p_pos.y = FLOOR_Y - PLAYER_H;
			p_speed.y = 0.f;
		}

		// update energy
		if(p_phase)
		{
			float energyFrameDrain = ENERGY_DRAIN * elapsed.asSeconds();
			if(p_energy < energyFrameDrain)
				p_phase = false;
			else
				p_energy -= energyFrameDrain;
		}
		else
		{
			p_energy += ENERGY_REGEN * elapsed.asSeconds();
			if(p_energy > ENERGY_MAX)
				p_energy = ENERGY_MAX;
		}

		// update fences
		auto it = fences.begin();
		while(it != fences.end())
		{
			if(*it < (p_pos.x - fence.getSize().x) && *it > lastScoredFence)
			{
				score += SCORE_FENCE;
				lastScoredFence = *it;
			}

			if(*it < (p_pos.x - PLAYER_OFFSET - fence.getSize().x))
				it = fences.erase(it);
			++it;
		}

		while(fences.size() < MAX_FENCES)
		{
			float offset = rand() % (MAX_BETWEEN_F - MIN_BETWEEN_F) + MIN_BETWEEN_F;
			offset += fence.getSize().x;
			fences.push_back(fences[fences.size()-1] + offset);
		}

		// update walls
		it = walls.begin();
		while(it != walls.end())
		{
			if(*it < (p_pos.x - wall.getSize().x) && *it > lastScoredWall)
			{
				score += SCORE_WALL;
				lastScoredWall = *it;
			}

			if(*it < (p_pos.x - PLAYER_OFFSET - wall.getSize().x))
				it = walls.erase(it);
			++it;
		}

		while(walls.size() < MAX_WALLS)
		{
			float offset = rand() % (MAX_BETWEEN_W - MIN_BETWEEN_W) + MIN_BETWEEN_W;
			offset += wall.getSize().x;
			walls.push_back(walls[walls.size()-1] + offset);
		}

		// test collisions

		/* dislpay scene */
        window.clear(sf::Color::White);
		camera.setCenter(W_W / 2 + p_pos.x - PLAYER_OFFSET, W_H / 2);
		window.setView(camera);

		floor.setPosition(p_pos.x - PLAYER_OFFSET, FLOOR_Y);
		window.draw(floor);
		
		player.setPosition(p_pos);
		window.draw(player);

		for(auto it = fences.begin(); it != fences.end(); ++it)
		{
			fence.setPosition(*it, FLOOR_Y - fence.getSize().y);
			sf::Color fenceColor = sf::Color::Red;
			if(p_phase)
				fenceColor.a = 125;
			fence.setFillColor(fenceColor);
			window.draw(fence);
		}
		
		for(auto it = walls.begin(); it != walls.end(); ++it)
		{
			wall.setPosition(*it, FLOOR_Y - wall.getSize().y);
			sf::Color wallColor = sf::Color::Black;
			if(p_phase)
				wallColor.a = 125;
			wall.setFillColor(wallColor);
			window.draw(wall);
		}

		// display hud
		if(hud)
		{
			scoreText.setString(std::to_string(score));
			window.setView(hudView);
			window.draw(scoreText);
		}

		// display energy bar
		sf::Vector2f barSize = energyBar.getSize();
		barSize.x = p_energy / ENERGY_MAX * ENERGY_BAR_W;
		energyBar.setSize(barSize);
		window.draw(energyBar);

        window.display();
    }

    return 0;
}
