#include <SFML/Graphics.hpp>

#define W_W 800
#define W_H 600
#define FLOOR_HEIGHT 150
#define FLOOR_Y W_H - FLOOR_HEIGHT
#define PLAYER_H 30
#define RATIO PLAYER_H/2


int main()
{
    sf::RenderWindow window(sf::VideoMode(W_W, W_H), "2+1D Runner");
	sf::RectangleShape floor(sf::Vector2f(1000.f, FLOOR_HEIGHT));
	floor.setPosition(0, FLOOR_Y);
	floor.setFillColor(sf::Color::Black);
	
	sf::RectangleShape player(sf::Vector2f(PLAYER_H, PLAYER_H));
	player.setFillColor(sf::Color::Blue);
	
	sf::Vector2f p_pos(2*PLAYER_H, FLOOR_Y - PLAYER_H * 3);
	sf::Vector2f p_speed(0.f, 0.f);
	sf::Vector2f g(0.f, 2*9.8f * RATIO);
	bool p_jump = false;

	sf::Clock clock;
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
			}
			if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Space)
				{
					p_speed.y = -10 * RATIO;
				}
			}
        }

		/* update game logic */
		sf::Time elapsed = clock.restart();

		// update player speed and position
		p_speed.y += g.y * elapsed.asSeconds();
		p_pos.y += p_speed.y * elapsed.asSeconds();
		// try collision with floor
		if(p_pos.y > FLOOR_Y - PLAYER_H)
		{
			p_pos.y = FLOOR_Y - PLAYER_H;
			p_speed.y = 0.f;
		}

        window.clear(sf::Color::White);

		/* dislpay scene */
		window.draw(floor);
		
		player.setPosition(p_pos); 
		window.draw(player);

        window.display();
    }

    return 0;
}
