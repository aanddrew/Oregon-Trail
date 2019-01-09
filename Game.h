#ifndef GAME_H
#define GAME_H

#include <string>
#include "Party.h"
#include "Wagon.h"
#include "Calendar.h"

using namespace std;

class Game
{
private:
	Party p;
	Calendar c;

	string fortNames[6];
	int fortMiles[6];
	string riverNames[4];
	int riverMiles[4];
	int riverDepths[4];

	int lastFort;
	int lastRiver;
public:
	Game();

	void play();

	Party initial();
	void shop(int);
	int doTurn();
};

#endif