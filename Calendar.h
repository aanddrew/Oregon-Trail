#ifndef CALENDAR_H
#define CALENDAR_H

#include <string>

using namespace std;

class Calendar
{
private:
	int day;
	int year;
	string monthNames[12];
	int monthDays[12];
public:
	Calendar();
	Calendar(int, int);

	void advance(int);
	int getDay();
	int getYear();
	string getDateString();
};

#endif