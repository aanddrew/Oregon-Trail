#include "Party.h"

using namespace std;

Party::Party()
{
	miles = 0;
	money = 1400.0;
	bullets = 0;
	oxen = 0;
	food = 0;

	extraWheels = 0;
	extraAxles = 0;
	extraTongues = 0;

	medicalKits = 0;

	lastFort = -1;
	lastRiver = -1;
}

Party::Party(string namesIn[5])
{
	//copy the array sent in to the data member "names" of party.
	for (int i = 0; i < 5; i++)
	{
		names[i] = namesIn[i];
		isAlive[i] = true;
		isSick[i] = false;
	}
	money = 1400.0;
	miles = 0;
	bullets = 0;
	oxen = 0;
	food = 0;

	extraWheels = 0;
	extraAxles = 0;
	extraTongues = 0;

	medicalKits = 0;

	lastFort = -1;
	lastRiver = -1;
}

string Party::getNameAt(int index)
{
	return names[index];
}

int Party::getMiles()
{
	return miles;
}

void Party::advance(int dist)
{
	miles += dist;
}

//money
float Party::getMoney() {return money;}
void Party::payMoney(float moneyIn) {money -= moneyIn;}


//party members
bool Party::isSickAt(int index) {return isSick[index];}
bool Party::isAliveAt(int index) {return isAlive[index];}

void Party::makeSick(int index) {isSick[index] = true;}
void Party::cure(int index) {isSick[index] = false;}

void Party::kill(int index) {isAlive[index] = false;}


//supplies
int Party::getFood() {return food;}
void Party::addFood(int foodIn) {food += foodIn;}

int Party::getOxen() {return oxen;}
void Party::addOxen(int oxenIn) {oxen += oxenIn;}

int Party::getBullets() {return bullets;}
void Party::addBullets(int bulletsIn) {bullets += bulletsIn;}

int Party::getExtraWheels() {return extraWheels;}
void Party::addExtraWheels(int wheelsIn) {extraWheels += wheelsIn;}

int Party::getExtraAxles() {return extraAxles;}
void Party::addExtraAxles(int axlesIn) {extraAxles += axlesIn;}

int Party::getExtraTongues() {return extraTongues;}
void Party::addExtraTongues(int tonguesIn) {extraTongues += tonguesIn;}

bool Party::repairWheel()
{
	if (wagon.getWheels() == 4) return false;
	else if (extraWheels > 0)
	{
		extraWheels--;
		wagon.setWheels(wagon.getWheels()+1);
	}
	else return false;
}
bool Party::repairAxle()
{
	if (wagon.getAxles() == 2) return false;
	else if (extraAxles > 0)
	{
		extraAxles--;
		wagon.setAxles(wagon.getAxles()+1);
		return true;
	}
	else return false;
}
bool Party::repairTongue()
{
	if (wagon.getTongues() == 2) return false;
	else if (extraTongues > 0)
	{
		extraTongues--;
		wagon.setTongues(wagon.getTongues()+1);
		return true;
	}
	else return false;
}

bool Party::breakWheel()
{
	if (wagon.getWheels() == 0) return false;
	else 
	{
		wagon.setWheels(wagon.getWheels()-1);
		return true;
	}
}
bool Party::breakAxle()
{
	if (wagon.getAxles() == 0) return false;
	else
	{
		wagon.setAxles(wagon.getAxles()-1);
		return true;
	}
}
bool Party::breakTongue()
{
	if (wagon.getTongues() == 0) return false;
	else
	{
		wagon.setTongues(0);
		return true;
	}
}

int Party::getLastFort() {return lastFort;}
void Party::setLastFort(int num) {lastFort = num;}

int Party::getLastRiver() {return lastRiver;}
void Party::setLastRiver(int num) {lastRiver = num;}