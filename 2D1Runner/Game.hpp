#pragma once

#include "conf.hpp"
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game(void);
	~Game(void);

	void Run(void);

private:
	void Init(void);
	void Reset(void);

	void ProcessEvents(void);
	void UpdateState(void);
	void Display(void);
	
	// Window && view
    sf::RenderWindow window;
	sf::View camera, hudView;

	// floor
	sf::RectangleShape floor;

	// score
	bool hud;
	sf::Font font;
	int score;
	sf::Text scoreText;
	
	// player
	sf::RectangleShape player;
	
	sf::Vector2f p_pos, p_speed, g;
	bool p_jump;

	float p_energy;
	bool p_phase;
	sf::RectangleShape energyBar;

	//obstables
	std::vector<float> fences;
	float lastScoredFence;
	sf::RectangleShape fence;

	std::vector<float> walls;
	float lastScoredWall;
	sf::RectangleShape wall;

	/* game loop */
	sf::Clock clock;
};

