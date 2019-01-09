#ifndef WAGON_H
#define WAGON_H

class Wagon
{
private:
	int wheels;
	int axles;
	int tongues;
public:
	Wagon();

	int getWheels();
	void setWheels(int);
	int getAxles();
	void setAxles(int);
	int getTongues();
	void setTongues(int);
};

#endif