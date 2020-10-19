#include "Game.h"
/*
This is implementation of old atari game called "Asteroids" by using SFML library.
Made by Ege Kutay YÜRÜŞEN (Decard Cain)
*/
//project started 21/8/2020
//project finished 4/9/2020
//project updated 4/9/2020
int main()
{
	std::srand((time(0)));
	Game game;
	game.run();
	return 0;
}
