/* Project Group:
Faye Jao		41133698
Kathy Chiang	55935182
Victor Hsiao	82542906
William Yang	83111331 */

#include <iostream>
#include <sstream>
#include <map>

#pragma once
class Game
{
public:
	Game();
	Game(std::string gameName, std::string player1IP, int player1ClientID);
	std::string getName();
	std::string getIP(int clientID);
	void readInput(int player, int newY, int newYspeed);
	void update();
	std::string sendBallData();
	bool gameFull();
	void addPlayer(int player, int clientID, std::string IP);
	void addPlayer2(int clientID, std::string IP);
	int getOpponent(int clientID);
	std::string sendOpponentPaddleData(int clientID);
	bool init();
	std::string sendScore();

	int getClientID(int num);

	/*Game(std::string ipAddress, std::string gameName);
	std::string getName();
	std::string getIP();
	void update();
	//void readInput(int value);
	void readInput(int newY, int newYspeed);
	void paddleMove(int x, int y);
	std::string sendBallData();
	bool paddleHitBall();
	bool paddleMissBall();
	std::string getHits();
	std::string getMisses();
	std::string getAccuracy();*/

private:
	std::string gameName = "";

	struct Ball
	{
		int x, y, x_speed, y_speed, radius;
	};

	struct Player
	{
		int player = 0;
		int clientID = -1;
		std::string IP = "";
		int x = -9001;
		int y = -9001;
		int width = 10;
		int height = 50;
		int x_speed = 0;
		int y_speed = 0;
		int score = 0;
		int opponent;
	};
	
	struct Wall
	{
		int left = 0;
		int right = 600;
		int top = 0;
		int bottom = 400;
	};

	Wall wall;
	Ball ball;
	Ball defaultBall;
	std::map<int, Player> player;
	bool initialized = 0;
	//Info info;
	/*int hits = 0;
	int misses = 0;
	int accuracy = 100;*/
	//int moveInt = 4;
};
