#include "Game.h"


//game objects


//private functions
void Game::initVar()
{

	window = nullptr;
	points = 0;
	health = 3;
	asteroidSpawnTimerMax = 100.f;
	asteroidSpawnTimer = 0.f;
	asteroidWaveMax = 6 ;
	multiplier = 60;
	asteroidMovSpeed = 0.7f;
	waveCount = 0;
	isShipDead = false;
	bulletCooldown = 0;
	fireBullet = false;
	pause = false;
}

void Game::initWindow()
{
	videomode.width = 1280;
	videomode.height = 720;
	window = new sf::RenderWindow(sf::VideoMode(videomode.width, videomode.height), "Astreoids", sf::Style::Close | sf::Style::Titlebar);
	window->setFramerateLimit(60);
}


void Game::initFonts()
{
	this->font.loadFromFile("Fonts/FFFFORWA.TTF");
}

void Game::initText()
{
	uiText.setFont(font);
	uiText.setCharacterSize(12);
	uiText.setFillColor(sf::Color::White);
	uiText.setString("None");
}

void Game::initBullet()
{
	int vertex = 36;
	bullet.setPointCount(vertex);
	int p = 0;
	int sc = 40;
	float tx = 0;
	float ty = 0;

	for (int i = 0; i < vertex; i++)
	{

		float x = sc * cos(p * PI / 180);
		float y = sc * sin(p * PI / 180);
		bullet.setPoint(i, sf::Vector2f(x, y));
		p += 36;
		tx += x;
		ty += y;
	}

	bullet.setOrigin(tx / vertex, ty / vertex);
}

void Game::initAsteroid()
{
	/* randomly setting up vertex positions with circular-noisy function
	then setting it's origin to object's middle
	setting colours
	and initial position on screen
	*/
	int vertex = 10;
	asteroid.setPointCount(vertex);
	int p = 0;
	int sc = 40;
	float tx = 0;
	float ty = 0;

	for (int i = 0; i < vertex; i++)
	{
		int k = rand() % 15 - 7;
		int l = rand() % 15 - 7;
		float x = sc * cos(p * PI / 180) + k;
		float y = sc * sin(p * PI / 180) + l;
		asteroid.setPoint(i, sf::Vector2f(x, y));
		p += 36;
		tx += x;
		ty += y;
	}
}
void Game::initShip()
{
	//initiliazing ship object
	ship.setPointCount(3);
	ship.setPoint(0, sf::Vector2f(0.f, 30.f));
	ship.setPoint(1, sf::Vector2f(0.f, 0.f));
	ship.setPoint(2, sf::Vector2f(42.f, 15.f));
	ship.setOrigin(sf::Vector2f(42 / 2.5, 45 / 3));
	ship.setScale(0.8f, 0.8f);
	ship.radius = 5;
	ship.id = "ship";
	ship.setPosition(sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y / 2));
	ship.setRotation(-90);
}
// constructor, destructor
Game::Game()
{
	initVar();
	initWindow();
	initAsteroid();
	initShip();
	initBullet();
	initFonts();
	initText();
}
Game::~Game()
{
	delete this->window;
}
//Accessors

const bool Game::running()
{
	return this->window->isOpen();
}

//public functions

void Game::spawnAsteroid(sf::Vector2f position, int Asize, float speed)
{
	/*
	our object(asteroid properties which are:
	each vertex positions are randomly generated to generate asteroid-like shape
	*/
	int vertex = 10;
	asteroid.setPointCount(vertex);
	int p = 0;
	float tx = 0;
	float ty = 0;
	for (int i = 0; i < vertex; i++)
	{
		int k = (rand() % 15 - 1) * Asize / 50;
		int l = (rand() % 15 - 1) * Asize / 50;
		float x = Asize * cos(p * PI / 180) + k;
		float y = Asize * sin(p * PI / 180) + l;
		asteroid.setPoint(i, sf::Vector2f(x, y));
		p += 36;
		tx += x;
		ty += y;
	}
	
	asteroid.radius = Asize;
	asteroid.setOrigin(tx / vertex, ty / vertex);
	asteroid.setFillColor(sf::Color::Black);
	asteroid.setOutlineColor(sf::Color::White);
	asteroid.setOutlineThickness(1.0f);
	asteroid.setRotation(rand() % 360);
	asteroid.setPosition(position);
	asteroid.thrust = speed;
	asteroid.id = "Asteroid";
	if (rand() % 101 == 41) 
	{
		asteroid.id = "sAsteroid";
		asteroid.setOutlineColor(sf::Color::Color(200,120,255,255));
		asteroid.thrust = speed * 4;
	}
	Asteroids.push_back(asteroid);
}
void Game::spawnWave()
{

	waveCount++;
	if(waveCount!=1)
	points += 20 * waveCount;
	asteroidMovSpeed = 0.75f * sqrt(waveCount);
	if (waveCount % 3 == 0)
		asteroidWaveMax+=2;
	sf::Vector2f asteroidPos;

	for (int i = 0; i < asteroidWaveMax; i++)
	{
		switch (rand() % 4)
		{
		case 0:
			asteroidPos = sf::Vector2f(
				this->window->getSize().x - rand() % this->window->getSize().x
				, 0.f);
			break;
		case 1:
			asteroidPos = sf::Vector2f(0.f
				, this->window->getSize().y - rand() % this->window->getSize().y);
			break;
		case 2:
			asteroidPos = sf::Vector2f(
				this->window->getSize().x - rand() % this->window->getSize().x,
				this->window->getSize().y);
		case 3:
			asteroidPos = sf::Vector2f(this->window->getSize().x,
				this->window->getSize().y - rand() % this->window->getSize().y);
			break;
		}
		spawnAsteroid(sf::Vector2f(asteroidPos), 50, asteroidMovSpeed);
	}
}

void Game::updateAsteroid()
{
	/* updates object(astaroid) per frame
	check if there is enough asteroid on the screen
	set up position of each asteroid per frame
	collision handling checkups with bullets
	*/

	if (Asteroids.size() == 0)
	{
		//new asteroid wave begins
		spawnWave();
		spawnShip();
	}
	for (auto& e : Asteroids)
	{
		e.move(cos(e.getRotation() * PI / 180) * e.thrust * dt * multiplier, sin(e.getRotation() * PI / 180) * e.thrust * dt * multiplier);
		wrapCoordinates(e.getPosition().x, e.getPosition().y, e);
	}
}

void Game::renderObject(std::vector<Object> obj)
{
	for (auto& e : obj)
		this->window->draw(e);
}
void Game::spawnShip()
{
	//setting ship's initial state 
	ship.setPosition(this->window->getSize().x / 2, this->window->getSize().y / 2);
	ship.dx = 0;
	ship.dy = 0;
	ship.setRotation(-90);
}
void Game::updateShip()
{
	/*updates ship position & rotation
		if ship is dead respawns if health is viable
		ship's movement function
	*/


	//the ship's speed limit.
	if (ship.dx * ship.dx + ship.dy * ship.dy > 25)
	{
		ship.dx += -cos(ship.getRotation() * PI / 180) * ship.thrust * dt * multiplier;
		ship.dy += -sin(ship.getRotation() * PI / 180) * ship.thrust * dt * multiplier;
	}
	ship.move(sf::Vector2f(ship.dx * dt * multiplier, ship.dy * dt * multiplier));
	wrapCoordinates(ship.getPosition().x, ship.getPosition().y, ship);

	//ship's collision mech
	if (isShipDead == false)
	{
		for (auto& e : Asteroids)
		{
			if (isObjectsCollide(e, ship))
			{
				isShipDead = true;
				spawnShip();
			}
		}
	}
	else
	{
		
		ship.timer += timeCounter;
		ship.setFillColor(sf::Color::Color(100, 100, 100, 128));
		if (ship.timer > 2.0f)
		{
			health--;
			ship.setFillColor(sf::Color::White);
			ship.timer = 0.f;
			isShipDead = false;
		}
	}
	if (health < 0)
		this->window->close();
}
void Game::renderShip()
{
	this->window->draw(ship);
}

void Game::spawnBullet(sf::Vector2f initPosition)
{
	// Spawns bullet and sets to the ship's position on the screen
	int vertex = 36;
	bullet.setPointCount(vertex);
	int p = 0;
	float sc = 2;
	float tx = 0;
	float ty = 0;
	for (int i = 0; i < vertex; i++)
	{

		float x = sc * cos(p * PI / 180);
		float y = sc * sin(p * PI / 180);
		bullet.setPoint(i, sf::Vector2f(x, y));
		p += 10;
		tx += x;
		ty += y;
	}
	
	bullet.radius = sc;
	bullet.setOrigin(tx / vertex, ty / vertex);
	bullet.setPosition(initPosition);
	bullet.setRotation(-90);
	bullet.rotate(ship.getRotation());
	Bullets.push_back(bullet);

}

void Game::updateBullet()
{
	//updates bullet's position over time and checks collision with asteroid
	bool bulletdeleted = false;
	bulletCooldown += dt;

	if (fireBullet)
	{
		spawnBullet(ship.getPosition());
		fireBullet = false;
		points += -1;
	}

	for (int i = 0; i < Bullets.size(); i++)
	{
		Bullets[i].timer += dt;
		Bullets[i].move(-sin(Bullets[i].getRotation() * PI / 180) * 13 * dt * multiplier, cos(Bullets[i].getRotation() * PI / 180) * 13 * dt * multiplier);
		wrapCoordinates(Bullets[i].getPosition().x, Bullets[i].getPosition().y, Bullets[i]);
		if (Bullets[i].timer > 1.0f)
		{
			Bullets.erase(Bullets.begin() + i);
		}
		//collision detection for asteroid vs bullet
		for (int j = 0; j < Asteroids.size() && i<Bullets.size(); j++)
			if (isObjectsCollide(Asteroids[j], Bullets[i]))
			{
				//bullet and asteroids collided, destroy both!!
				if(Asteroids[j].id=="sAsteroid")
					points+=20*waveCount;

				points += 16;
				if (Asteroids[j].radius > 20)
				{
					spawnAsteroid(Asteroids[j].getPosition(), Asteroids[j].radius / 1.7f, asteroidMovSpeed);
					spawnAsteroid(Asteroids[j].getPosition(), Asteroids[j].radius / 1.7f, asteroidMovSpeed);
				}
				//creating new asteroids right before lose the data of asteroid's location
				

				Bullets.erase(Bullets.begin() + i);
				Asteroids.erase(Asteroids.begin() + j);
			}
		//bullet timer for erasing itself
	}
}
void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << points << "\n" << "Health: x" << health << "\n" << "Wave :" << waveCount;
	uiText.setString(ss.str());
	uiText.setCharacterSize(12);
	uiText.setPosition(10, 10);
	if (pause) 
	{
		ss << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Paused";
		uiText.setString(ss.str());
	}
	
	

}

void Game::renderText(sf::RenderTarget& window)
{
	window.draw(uiText);
}

bool Game::isCircleinsideCircle(float ax, float ay, float aradius, float x, float y,float radius)
{
	return sqrt((x - ax) * (x - ax) + (y - ay) * (y - ay)) < aradius+radius;
}

bool Game::isPointinsideCircle(float ax, float ay, float radius, float x, float y)
{
	return sqrt((x - ax) * (x - ax) + (y - ay) * (y - ay)) < radius;
}
bool Game::isObjectsCollide(Object circle, Object obj)
{
	if (obj.radius == 0)
	{
		if (isPointinsideCircle(circle.getPosition().x, circle.getPosition().y, circle.radius, obj.getPosition().x, obj.getPosition().y))
			return true;
		else
			return false;
	}
	else
		if (isCircleinsideCircle(circle.getPosition().x, circle.getPosition().y, circle.radius, obj.getPosition().x, obj.getPosition().y, obj.radius))
			return true;
		else
			return false;
}

void Game::wrapCoordinates(int ix, int iy, sf::Shape& e)
{
	if (ix < 0.0f)
		e.setPosition(ix + (float)this->window->getSize().x, iy);
	else if (ix > (float)this->window->getSize().x)
		e.setPosition(ix - (float)this->window->getSize().x, iy);
	else if (iy < 0.0f)
		e.setPosition(ix, iy + (float)this->window->getSize().y);
	else if (iy > (float)this->window->getSize().y)
		e.setPosition(ix, iy - (float)this->window->getSize().y);
}


void Game::pollEvents()
{
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
			//Keyboard inputs:
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
			{
				window->close();
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{

				if (!pause)
					pause = true;
				else
					pause = false;
			}
		}
	}
}
void Game::UserInputs()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		ship.rotate(-3.0f * dt * multiplier);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		ship.rotate(3.0f * dt * multiplier);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		ship.dx += cos(ship.getRotation() * PI / 180) * ship.thrust * dt * multiplier;
		ship.dy += sin(ship.getRotation() * PI / 180) * ship.thrust * dt * multiplier;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (bulletCooldown > 0.50)
		{
			bulletCooldown = 0;
			fireBullet = true;
		}
	}
}
void Game::run()
{
	while (running())
	{
		//update
		
		update();
		//render
		render();
	}
}
void Game::update()
{
	dt = clock.restart().asSeconds();
	timeCounter = dt;
	pollEvents();
	
	if (!pause) 
	{
		UserInputs();
		updateShip();
		updateBullet();
		updateAsteroid();
	}
	updateText();
	

}

void Game::render()
{
	window->clear();
	//Draw game objects
	
	renderShip();
	renderObject(Bullets);
	renderObject(Asteroids);
	renderText(*this->window);

	this->window->display();
}
