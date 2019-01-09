#include <iostream>
#include <fstream>

#include "Calendar.h"

using namespace std;

Calendar::Calendar()
{
	Calendar(0, 0);
}

Calendar::Calendar(int dayIn, int yearIn)
{
	day = dayIn;
	year = yearIn;

	string s;
	ifstream months("Month_Names.txt");
	int i = 0;
	while (getline(months, s))
	{
		monthNames[i] = s;
		i++;
	}

	monthDays[0]  = 31;
	monthDays[1]  = 28;
	monthDays[2]  = 31;
	monthDays[3]  = 30;
	monthDays[4]  = 31;
	monthDays[5]  = 30;
	monthDays[6]  = 31;
	monthDays[7]  = 31;
	monthDays[8]  = 30;
	monthDays[9]  = 31;
	monthDays[10] = 30;
	monthDays[11] = 31;
}

void Calendar::advance(int daysIn)
{
	if (day + daysIn > 365)
	{
		day = (day+daysIn) % 365;
		year++;
	}
	else
	{
		day += daysIn;
	}
}

string Calendar::getDateString()
{
	bool dayFound = false;
	int monthNum = 0;
	int dayTemp = day;
	while (!dayFound)
	{
		if (dayTemp > monthDays[monthNum])
		{
			dayTemp -= monthDays[monthNum];
			monthNum++;
		}
		else
		{
			dayFound = true;
		}
	}
	string returned = "";
	returned += to_string(monthNum + 1);
	returned += "-";
	returned += to_string(dayTemp);
	returned += "-";
	returned += to_string(year);

	return returned;
}