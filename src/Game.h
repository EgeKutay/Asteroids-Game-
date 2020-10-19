#pragma once

#include <ctime>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>


const float PI = 3.14;

class Game
{
private:

	struct Object :public sf::ConvexShape

	{
		std::string id="";
		float radius = 0.f;
		float dx = 0.f;
		float dy = 0.f;
		float thrust = 0.052f;
		float timer = 0.f;
		Object()
		{
			id;
			id;
			timer;
			dx;
			dy;
			thrust;
		}
	};


	//general video objects
	sf::VideoMode videomode;
	sf::RenderWindow* window;
	sf::Event event;
	sf::Clock clock;
	//resources
	sf::Font font;
	sf::Text uiText;

	

	//private functions
	void initVar();
	void initWindow();

	//initObjects
	void initFonts();
	void initText();
	void initBullet();
	void initAsteroid();
	void initShip();
	//Game logic
	bool pause;
	int multiplier;
	float dt;
	int points;
	int health;
	int waveCount;
	float asteroidMovSpeed;
	float timeCounter;
	bool isShipDead;
	float asteroidSpawnTimer;
	float asteroidSpawnTimerMax;
	float asteroidWaveMax;
	float bulletCooldown;
	bool fireBullet;
	// game objects:
	Object asteroid;
	Object ship;
	Object bullet;
	std::vector<Object> Bullets;
	std::vector<Object> Asteroids;
public:

	//    Constructor/Destructor
	Game();
	virtual ~Game();
	//-----ACCESSORS------
	const bool running();

	//------PUBLIC FUNCTIONS-------

	//object(asteroid) function
	void spawnWave();
	void spawnAsteroid(sf::Vector2f position, int size, float speed);
	void updateAsteroid();
	void renderObject(std::vector<Object> ArrayList);
	//object(ship)
	void spawnShip();
	void updateShip();
	void renderShip();
	//object (bullet)
	void spawnBullet(sf::Vector2f initialPosition);
	void updateBullet();
	//UI
	void updateText();
	void renderText(sf::RenderTarget& window);
	//collision detections and coordinating
	bool isCircleinsideCircle(float ax, float ay, float aradius, float x, float y,float radius);
	bool isPointinsideCircle(float ax, float ay, float radius, float x, float y);
	bool isObjectsCollide(Object obj1, Object obj2);
	void wrapCoordinates(int ix, int iy, sf::Shape& e);
	//user inputs
	void pollEvents();
	void UserInputs();

	//Screen update and render
	void run();
	void update();
	void render();
};

