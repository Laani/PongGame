#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include <map>
#include <windows.h>
#include "Game.h"
using namespace std;

webSocket server;

map<int, string> clientToGameID; //clientID -> Game
map<string, Game> gameIDToGame;


/* called when a client connects */
void openHandler(int clientID)
{
	/*std::cout << clientID << std::endl;
	ostringstream as,os;
	os << server.getClientIP(clientID) << " has joined.";
	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++){
	if (clientIDs[i] != clientID)
	server.wsSend(clientIDs[i], os.str());
	*/
	//  server.wsSend(clientID, "Welcome!");

	//as << server.getClientIP(clientID);
	//server.wsSend(clientID, as.str());
	//as.flush();
	//std::cout << "something here" << std::endl;

}

/* called when a client disconnects */
void closeHandler(int clientID){
	/*ostringstream os;
	os << "Stranger " << clientID << " has left.";

	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++){
	if (clientIDs[i] != clientID)
	server.wsSend(clientIDs[i], os.str());
	}*/

	//go to game, find other player, send game over. 
	int opp = gameIDToGame[clientToGameID[clientID]].getOpponent(clientID);
	server.wsSend(opp, "playerdc"); //#faye
	gameIDToGame.erase(clientToGameID[clientID]);

	clientToGameID.erase(opp);
	clientToGameID.erase(clientID);
	//server.wsClose(clientID);
	//server.wsClose(opp);

	//std::cout << "somebody left" << std::endl;
	/*vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++){
	std::cout << "client " << server.getClientIP(i) << " is still here" << std::endl;
	}*/
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message)
{
	if (message.empty())
	{
		std::cout << "message is empty" << std::endl;
	}
	else
	{
		//std::cout<<server.getClientIP(clientID)<<" " << message << std::endl;
		//int found = message.find("gameid");
		if (message.find("gameid") != string::npos)
		{

			string gameID = message.substr(7);

			if (gameIDToGame[gameID].gameFull() == 1) //implement #faye
			{
				ostringstream os, as;
				//std::cout << "player 3 tried to join a full game" << std::endl;
				os << "Game lobby '" << gameID << "' is already full. Please pick a new name.";
				as << "disconnect (server)";
				server.wsSend(clientID, os.str());
				server.wsSend(clientID, as.str());
				os.flush();
				as.flush();
			}
			else if (gameIDToGame[gameID].init() == 0)
			{
				ostringstream os;
				clientToGameID[clientID] = gameID;
				gameIDToGame[gameID] = Game{ gameID, server.getClientIP(clientID), clientID }; //change this to init game #faye
				os << "one ";
				std::cout << "sent client " << clientID << "'one'.\n";
				server.wsSend(clientID, os.str());
				os.flush();
			}
			else if (gameIDToGame[gameID].init() == 1)
			{

				ostringstream os, as;
				clientToGameID[clientID] = gameID;
				//std::cout << "client " << clientID << "has joined " << gameID << std::endl;
				//gameIDToGame[gameID] = Game{ server.getClientIP(clientID), gameID }; //fix init #faye
				gameIDToGame[gameID].addPlayer2(clientID, server.getClientIP(clientID));
				server.wsSend(clientID, gameIDToGame[gameID].sendBallData());
				os << "two ";
				server.wsSend(clientID, os.str());
				std::cout << "sent client " << clientID << " 'two'.\n";
				as << "ready ";

				int opponentClientID = gameIDToGame[clientToGameID[clientID]].getOpponent(clientID);
				
				
				
				server.wsSend(opponentClientID, as.str());
				server.wsSend(clientID, as.str());
				server.wsSend(opponentClientID, gameIDToGame[gameID].sendBallData());

				
				os.flush();
				as.flush();

			}
		}
	}
	if (message.find("paddle1") != string::npos)
	{
		//std::cout << message << std::endl;
		string thing = message.substr(8);
		string ypos = thing.substr(0, thing.find(" "));
		string yspeed = thing.substr(thing.find(" ") + 1);

		//std::cout << ypos << " " << yspeed << std::endl;
		int value = atoi(ypos.c_str());
		int value2 = atoi(yspeed.c_str());


		gameIDToGame[clientToGameID[clientID]].readInput(1, value, value2); //#faye

		int opponentClientID = gameIDToGame[clientToGameID[clientID]].getOpponent(clientID);

		server.wsSend(clientID, gameIDToGame[clientToGameID[clientID]].sendBallData());
		server.wsSend(opponentClientID, gameIDToGame[clientToGameID[clientID]].sendBallData());
		server.wsSend(clientID, gameIDToGame[clientToGameID[clientID]].sendOpponentPaddleData(clientID));
		server.wsSend(clientID, gameIDToGame[clientToGameID[clientID]].sendScore());
		server.wsSend(opponentClientID, gameIDToGame[clientToGameID[clientID]].sendScore());
		//send to other player as well #faye

	}
	else if (message.find("paddle2") != string::npos)
	{
		//std::cout << message << std::endl;
		string thing = message.substr(8);
		string ypos = thing.substr(0, thing.find(" "));
		string yspeed = thing.substr(thing.find(" ") + 1);

		//std::cout << ypos << " " << yspeed << std::endl;
		int value = atoi(ypos.c_str());
		int value2 = atoi(yspeed.c_str());

		int opponentClientID = gameIDToGame[clientToGameID[clientID]].getOpponent(clientID);

		gameIDToGame[clientToGameID[clientID]].readInput(2, value, value2); //need input reading for paddle 2 #faye

		server.wsSend(opponentClientID, gameIDToGame[clientToGameID[clientID]].sendBallData());
		server.wsSend(clientID, gameIDToGame[clientToGameID[clientID]].sendBallData());
		server.wsSend(clientID, gameIDToGame[clientToGameID[clientID]].sendOpponentPaddleData(clientID));
		server.wsSend(clientID, gameIDToGame[clientToGameID[clientID]].sendScore());
		server.wsSend(opponentClientID, gameIDToGame[clientToGameID[clientID]].sendScore());
		//#faye
	}

}


double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}


/* called once per select() loop */
void periodicHandler(){
	static time_t next = GetCounter() + (1000 / 60);
	time_t current = GetCounter();


	if (current >= next){
		// ostringstream os;
		//      string timestring = ctime(&current);
		//      timestring = timestring.substr(0, timestring.size() - 1);
		//      os << timestring;
		////os << x << std::endl;
		if (gameIDToGame.empty())
		{
		}
		else
		{
			for (std::map<string, Game>::iterator it = gameIDToGame.begin(); it != gameIDToGame.end(); ++it)
			{
				if (it->second.gameFull())
				{
					it->second.update();
					int p1 = it->second.getClientID(1);
					int p2 = it->second.getClientID(2);
					server.wsSend(p1, gameIDToGame[clientToGameID[p1]].sendBallData());
					server.wsSend(p2, gameIDToGame[clientToGameID[p2]].sendBallData());

					server.wsSend(p1, gameIDToGame[clientToGameID[p1]].sendOpponentPaddleData(p1));
					server.wsSend(p2, gameIDToGame[clientToGameID[p2]].sendOpponentPaddleData(p2));

					server.wsSend(p1, gameIDToGame[clientToGameID[p1]].sendScore());
					server.wsSend(p2, gameIDToGame[clientToGameID[p2]].sendScore());
				}

				//server.wsSend(i, os.str());
				//server.wsSend(i, gameIDToGame[clientToGameID[clientIDs[i]]].sendBallData());
				//std::cout << gameIDToGame[i].sendBallData() << std::endl;
			}
			//vector<int> clientIDs = server.getClientIDs();
			//for (int i = 0; i < clientIDs.size(); i++)
			//{
			//	//std::cout << "GameID: " << clientToGameID[clientIDs[i]] << "; ClientID: " << clientIDs[i] << std::endl;
			//	if (clientIDs[i] != NULL)
			//	{
			//		//std::cout << i << " " << gameIDToGame[clientToGameID[i]].getOpponent() << std::endl;

			//		server.wsSend(clientIDs[i], gameIDToGame[clientToGameID[clientIDs[i]]].sendBallData());
			//		//int opponentClientID = gameIDToGame[clientToGameID[clientID]].getOpponent(clientID);
			//		server.wsSend(clientIDs[i], gameIDToGame[clientToGameID[clientIDs[i]]].sendOpponentPaddleData(i));
			//		server.wsSend(clientIDs[i], gameIDToGame[clientToGameID[clientIDs[i]]].sendScore());
			//	}

			//}
		}

		next = GetCounter() + (1000 / 60);
	}
}

int main(int argc, char *argv[]){
	int port = 9001;
	StartCounter();
	// cout << "Please set server port: ";
	//cin >> port;

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port);

	return 1;
}
