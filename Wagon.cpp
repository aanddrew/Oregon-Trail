#include <iostream>
#include "Wagon.h"

using namespace std;

Wagon::Wagon()
{
	wheels = 4;
	axles  = 2;
	tongues = 1;
}

int Wagon::getWheels() {return wheels;}
void Wagon::setWheels(int wheelsIn) {wheels = wheelsIn;} 

int Wagon::getAxles() {return axles;}
void Wagon::setAxles(int axlesIn){axles = axlesIn;}

int Wagon::getTongues() {return tongues;}
void Wagon::setTongues(int tonguesIn) {tongues = tonguesIn;}