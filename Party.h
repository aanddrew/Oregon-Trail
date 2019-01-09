#ifndef PARTY_H
#define PARTY_H

#include <string>
#include "Wagon.h"

using namespace std;

class Party
{
private:
	string names[5];
	bool isAlive[5];
	bool isSick[5];
	float money;
	int miles;
	int food;
	int oxen;
	int bullets;

	int extraWheels;
	int extraAxles;
	int extraTongues;

	int medicalKits;

	Wagon wagon;

	int lastFort;
	int lastRiver;
public:
	Party();
	Party(string[5]);

	//people in the party

	string getNameAt(int);

	bool isSickAt(int);
	bool isAliveAt(int);

	void makeSick(int);
	void cure(int);

	void kill(int);

	//miles and money

	int getMiles();
	void advance(int);

	float getMoney();
	void payMoney(float);

	//general supplies

	int getFood();
	void addFood(int);

	int getOxen();
	void addOxen(int);

	int getBullets();
	void addBullets(int);

	int getExtraWheels();
	void addExtraWheels(int);

	int getExtraAxles();
	void addExtraAxles(int);

	int getExtraTongues();
	void addExtraTongues(int);

	//reparing and breaking

	bool repairWheel();
	bool repairAxle();
	bool repairTongue();

	bool breakWheel();
	bool breakAxle();
	bool breakTongue();

	//forts and rivers

	int getLastFort();
	void setLastFort(int);

	int getLastRiver();
	void setLastRiver(int);
};

#endif