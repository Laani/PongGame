/* Project Group:
Faye Jao		41133698
Kathy Chiang	55935182
Victor Hsiao	82542906
William Yang	83111331 */

#include "Game.h"

Game::Game()
{}

Game::Game(std::string gameName, std::string player1IP, int player1ClientID)
{
	Game::gameName = gameName;
	Game::defaultBall = Game::Ball{ 200, 190, 3, 0, 5 };
	Game::ball = Game::Ball{ Game::defaultBall.x, Game::defaultBall.y, Game::defaultBall.x_speed, Game::defaultBall.y_speed, Game::defaultBall.radius };

	Game::player[1];
	Game::player[1].player = 1;
	Game::player[1].x = 580;
	Game::player[1].y = 175;
	Game::player[1].opponent = 2;
	Game::initialized = 1;
	Game::player[2];
	Game::player[2].player = 2;
	Game::player[2].x = 10;
	Game::player[2].y = 175;
	Game::player[2].opponent = 1;

	Game::addPlayer(1, player1ClientID, player1IP);
}

bool Game::init()
{
	return initialized;
}

std::string Game::getName()
{
	return Game::gameName;
}

std::string Game::getIP(int clientID)
{
	return Game::player[clientID].IP;
}

// paddleMove()
/*void Game::paddleMove(int x, int y)
{
	Game::player[1].x += x;
	Game::player[1].y += y;
	Game::player[1].x_speed = x;
	Game::player[1].y_speed = y;
	if (Game::player[1].y < 0) // all the way to the top
	{
		Game::player[1].y = 0;
		Game::player[1].y_speed = 0;
	}
	else if (Game::player[1].y + Game::player[1].height > 400) // all the way to the bottom
	{
		Game::player[1].y = Game::wall.bottom - Game::player[1].height;
		Game::player[1].y_speed = 0;
	}
}*/

void Game::readInput(int player, int newY, int newYspeed)
{
	Game::player[player].y = newY;
	Game::player[player].y_speed = newYspeed;
	if (Game::player[player].y < Game::wall.top) // all the way to the top
	{
		Game::player[player].y = 0;
		Game::player[player].y_speed = 0;
	}
	else if (Game::player[player].y + Game::player[player].height > Game::wall.bottom) // all the way to the bottom
	{
		Game::player[player].y = Game::wall.bottom - Game::player[player].height;
		Game::player[player].y_speed = 0;
	}
}

//void Game::readInput(int value)
//{
//	if (value == 87) // move up
//		Game::paddleMove(0, -Game::moveInt);
//	else if (value == 83) // move down
//		Game::paddleMove(0, Game::moveInt);
//}

void Game::update()
{
	Game::ball.x += Game::ball.x_speed;
	Game::ball.y += Game::ball.y_speed;

	int top_x = Game::ball.x - Game::ball.radius;
	int top_y = Game::ball.y - Game::ball.radius;
	int bottom_x = Game::ball.x + Game::ball.radius;
	int bottom_y = Game::ball.y + Game::ball.radius;

	if (Game::ball.y - Game::ball.radius < Game::wall.top) // hitting the top wall
	{
		Game::ball.y = Game::ball.radius;
		Game::ball.y_speed = -Game::ball.y_speed;
	}
	else if (Game::ball.y + Game::ball.radius > Game::wall.bottom) // hitting the bottom wall
	{
		Game::ball.y = (Game::wall.bottom - Game::ball.radius);
		Game::ball.y_speed = -Game::ball.y_speed;
	}

	// THIS IS TEMPORARILY FOR PART ONE:
	/*else if (Game::ball.x - Game::ball.radius < Game::wall.left) // hitting the left wall
	{
		Game::ball.x = Game::ball.radius;
		Game::ball.x_speed = -Game::ball.x_speed;
	}*/

	if ((Game::ball.x > Game::wall.right) || (Game::ball.x < Game::wall.left)) // somebody loses
	{
		if (Game::ball.x > Game::wall.right)
		{
			Game::player[2].score++;
			Game::ball.x_speed = Game::defaultBall.x_speed;
		}
		else if (Game::ball.x < Game::wall.left)
		{
			Game::player[1].score++;
			Game::ball.x_speed = -Game::defaultBall.x_speed;
		}
		Game::ball.y_speed = Game::defaultBall.y_speed;
		Game::ball.x = Game::defaultBall.x;
		Game::ball.y = Game::defaultBall.y;
	}
	
	if (top_x > (Game::wall.right / 2)) // hit player 1, original value 300
	{
		if (top_y < (Game::player[1].y + Game::player[1].height) &&
			bottom_y > Game::player[1].y &&
			top_x < (Game::player[1].x + Game::player[1].width) && 
			bottom_x > Game::player[1].x)
		{
			Game::ball.x_speed = -Game::defaultBall.x_speed;
			Game::ball.y_speed += (Game::player[1].y_speed / 2);
			Game::ball.x += Game::ball.x_speed;
		}
	}
	else // hit player 2
	{
		if (top_y < (Game::player[2].y + Game::player[2].height) &&
			bottom_y > Game::player[2].y &&
			top_x < (Game::player[2].x + Game::player[2].width) &&
			bottom_x > Game::player[2].x)
		{
			Game::ball.x_speed = 3;
			Game::ball.y_speed += (Game::player[2].y_speed / 2);
			Game::ball.x += Game::ball.x_speed;
		}
	}

	// The accuracy stuff
	/*if (Game::player[1].score + Game::misses == 0)
	{
		Game::accuracy = 100;
	}
	else
	{
		Game::accuracy = (int)round(((double)Game::player[1].score / ((double)Game::player[1].score + (double)Game::misses)) * 100);
		//std::cout << accuracy << std::endl;
	}*/
}

// Hard-coded version of update()
/*void Game::update()
{
	Game::ball.x += Game::ball.x_speed;
	Game::ball.y += Game::ball.y_speed;
	int top_x = Game::ball.x - 5;
	int top_y = Game::ball.y - 5;
	int bottom_x = Game::ball.x + 5;
	int bottom_y = Game::ball.y + 5;

	if (Game::ball.y - 5 < 0) { // hitting the top wall
		Game::ball.y = 5;
		Game::ball.y_speed = -Game::ball.y_speed;
	}
	else if (Game::ball.y + 5 > 400) { // hitting the bottom wall
		Game::ball.y = 395;
		Game::ball.y_speed = -Game::ball.y_speed;
	}

	// THIS IS TEMPORARILY FOR PART ONE:
	else if (Game::ball.x - 5 < 0) { // hitting the left wall
		Game::ball.x = 5;
		Game::ball.x_speed = -Game::ball.x_speed;
	}

	if (Game::ball.x > 600) { // the ball was lost
		Game::ball.x_speed = 3;
		Game::ball.y_speed = 0;
		Game::ball.x = 200;
		Game::ball.y = 190;
		Game::misses++;
	}

	if (top_x > 300) {
		// hit player 1
		if (top_y < (Game::player[1].y + Game::player[1].height) && bottom_y > Game::player[1].y && top_x < (Game::player[1].x + Game::player[1].width) && bottom_x > Game::player[1].x) {
			Game::ball.x_speed = -3;
			Game::ball.y_speed += (Game::player[1].y_speed / 2);
			//std::cout << Game::player[1].y_speed << std::endl;
			Game::ball.x += Game::ball.x_speed;
			Game::player[1].score++;
			std::cout << "ball hit paddle" << std::endl;
		}
	}

	if (Game::player[1].score + Game::misses == 0)
	{
		Game::accuracy = 100;
	}
	else
	{
		Game::accuracy = Game::player[1].score / (Game::player[1].score + Game::misses) * 100;
	}
};*/

/*bool Game::paddleMissBall()
{
	if (Game::ball.x > Game::wall.right) // the ball was lost
	{
		return true;
	}
	return false;
}*/

/*bool Game::paddleHitBall()
{
	int top_x = Game::ball.x - Game::ball.radius;
	int top_y = Game::ball.y - Game::ball.radius;
	int bottom_x = Game::ball.x + Game::ball.radius;
	int bottom_y = Game::ball.y + Game::ball.radius;

	if (top_x > (Game::wall.right / 2)) // hit player 1, original value 300
	{
		if (top_y < (Game::player[1].y + Game::player[1].height) &&
			bottom_y > Game::player[1].y &&
			top_x < (Game::player[1].x + Game::player[1].width)
			&& bottom_x > Game::player[1].x)
		{
			return true;
		}
	}
	return false;
}*/

std::string convert(int integer)
{
	std::ostringstream c;
	c.flush();
	c << integer;
	return c.str();
}

// Package formatting: ball ballX ballY ballX_speed ballY_speed
std::string Game::sendBallData()
{
	std::string string = "ball "+ convert(Game::ball.x) + " " + convert(Game::ball.y);
	string += " " +convert(Game::ball.x_speed) + " " + convert(Game::ball.y_speed);
	//string += convert(Game::player[1].score) + " " + convert(Game::misses) + " " + convert(Game::accuracy);
	return string;
}

// More accuracy stuff
/*std::string Game::getHits()
{
	std::string string ="hits "+ convert(Game::player[1].score);
	return string;
}
std::string Game::getMisses()
{
	std::string string = "miss " + convert(Game::misses);
	return string;
}
std::string Game::getAccuracy()
{
	std::string string = "a " + convert(Game::accuracy);
	
	return string;
}*/

bool Game::gameFull()
{
	return (Game::player[1].IP != "") && (Game::player[2].IP != "");
}

void Game::addPlayer(int player, int clientID, std::string IP)
{
	Game::player[player].clientID = clientID;
	Game::player[player].IP = IP;
}

void Game::addPlayer2(int clientID, std::string IP)
{
	addPlayer(2, clientID, IP);
}

int Game::getOpponent(int clientID)
{
	for (int i = 1; i <= 2; i++)
	{
		if (Game::player[i].clientID == clientID)
		{
			return Game::player[Game::player[i].opponent].clientID;
		}
	}
}


std::string Game::sendOpponentPaddleData(int clientID)
{
	for (int i = 1; i <= 2; i++)
	{
		if (Game::player[i].clientID == clientID)
		{
			return "paddle " + convert(Game::player[Game::player[i].opponent].y);
		}
	}
}

std::string Game::sendScore()
{
	//std::cout << "score " + convert(Game::player[1].score) + " " + convert(Game::player[2].score) << std::endl;
	return "score " + convert(Game::player[1].score) + " " + convert(Game::player[2].score);
}

int Game::getClientID(int num)
{
	return player[num].clientID;
}