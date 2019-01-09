#include <iostream>
#include "Game.h"
#include <cstdlib>

#include <fstream>

using namespace std;

Game::Game()
{
	srand(time(0));
	c = Calendar(60,1847);

	//initialize the rivers and forts and junk...
	ifstream  fort_milestones("fort-milestones.txt");
	ifstream river_milestones("river-milestones.txt");

	string s;
	int i = 0;
	while (getline(fort_milestones, s))
	{
		//if we are on an even line
		//even lines are names
		if (i%2 == 0)
		{
			fortNames[i/2] ="";
			for (int j = 0; j < s.length()-1; j++)
				fortNames[i/2] += s[j];
		}
		//odd lines are numbers
		else
		{
			fortMiles[(i-1)/2] = stoi(s);
		}
		i++;
	}

	i = 0;
	while(getline(river_milestones, s))
	{
		//even lines are names
		if (i%2 == 0)
		{
			riverNames[i/2] ="";
			for (int j = 0; j < s.length()-1; j++)
				riverNames[i/2] += s[j];
		}
		else
		{
			//traverse the string
			int j;
			string temp = "";
			for (j = 0; j < s.length(); j++)
			{
				if (s[j] == 'm') break;
				else
				{
					temp += s[j];
				}
			}
			riverMiles[(i-1)/2] = stoi(temp);

			j+= 3;
			temp = "";
			for (; j < s.length(); j++)
			{
				if (s[j] != ' ')
				{
					if (s[j] == 'f' || s[j] == 't') ;
					else temp += s[j];
				}
			}
			riverDepths[i] = stoi(temp);
		}
		i++;
	}

	initial();
}

void Game::play()
{
	//create this to exit the loop;
	bool stillPlaying = true;
	//declare this here so that I don't have to create it over and over again.
	int x;
	while (stillPlaying)
	{
		x = doTurn();
		switch(x)
		{
			case 4:
			{
				cout << "Bye! Thanks for playing!" << endl;
				stillPlaying = false;
			}
				break;
			case 5:
			{
				cout << "Congratulations! You reached the end of the Oregon trail!" << endl;
				stillPlaying = false;
			}
				break;
			case 6:
			{
				cout << "You are unable to continue on the oregon trail, you lose!" << endl;
				stillPlaying = false;
			}
			break;
		}
	}
	//game over..
}

//the initial setup of the game. Asks player for the names of their party etc..
Party Game::initial()
{
	//GETTING THE NAMES FOR THE PARTY
	string names[5];
	cout << "What is the first name of the wagon leader?" << endl;
	cin >> names[0];

	cout << "What are the first names of the four other members in your party?" << endl;
	cout << "1. " << names[0] << endl;
	for (int i = 1; i < 5; i++)
	{
		cout << i+1 << ". ";
		cin >> names[i];
	}
	while (true)
	{
		cout << "Are these names correct? (Y/N)" << endl;
		string temp;
		cin >> temp;
		if (temp == "Y" || temp == "y") break;
		else
		{
			cout << "Change which name?" << endl;
			cin >> temp;
			int n;
			if (n = stoi(temp) - 1)
			{
				cout << "What is the correct name?" << endl;
				cin >> names[n];

				cout << "Here are the current names: " << endl;
				for (int i = 0; i < 5; i++)
				{
					cout << i+1 << ". " << names[i] << endl;
				}
			}
		}
	}
	//END GETTING NAMES FOR PARTY

	//initial buying of supplies
	//first we will print an excerpt about 
	cout << "It is 1847. Your jumping off place for Oregon is Independence, Missouri." << endl;
	ifstream storeInfo("store_info.txt");
	string temp;
	//print the info about the oregon trail and shopping and whatnot...
	while (getline(storeInfo, temp))
	{
		cout << temp << endl;
	}

	//construct the party
	p = Party(names);

	//now throw them into the level zero shop.
	shop(0);

	cout << "You may leave any date between 03/01/1847 and 05/01/1847" << endl;
	cout << "Would you like to:" << endl;
	cout << "\t1. Leave on the default date of 03/28/1847?" << endl;
	cout << "\t2. Choose your own date?" << endl;
	cin >> temp;
	int choice = stoi(temp);
	switch(choice)
	{
		case 1:
		{
			cout << "You are leaving on 03/28/1847" << endl;
			c = Calendar(87,1847);
		}
			break;
		case 2:
		{
			cout << "Would you like to leave in:" << endl;
			cout << "\t1. March?" << endl;
			cout << "\t2. April?" << endl;
			cin >> temp;
			choice = stoi(temp);
			int day;
			bool validDay = false;
			while (!validDay)
			{
				cout << "Which day would you like to leave?" << endl;
				cin >> temp;
				day = stoi(temp);
				switch(choice)
				{
					case 1:
					{
						if (day > 31 || day < 1)
						{
							cout << "Not a valid day." << endl;
						}
						else
						{
							c = Calendar(59 + day, 1847);
							validDay = true;
						}
					}
						break;
					case 2:
					{
						if (day > 30 || day < 1)
						{
							cout << "Not a valid day." << endl;
						}
						else
						{
							c = Calendar(90 + day, 1847);
							validDay = true;
						}
					}
						break;
				}
			}
		}
			break;
	}

	return p;
}

void Game::shop(int storeNum)
{
	string temp;
	int choice;
	bool stillBuying = true;
	while (stillBuying)
	{
		cout << "What would you like to buy?" << endl;
		cout << "You have $" << p.getMoney() << endl;
		cout << "1. Oxen" << endl;
		cout << "2. Food" << endl;
		cout << "3. Bullets" << endl;
		cout << "4. Miscellaneous supplies" << endl;
		cout << "5. Done. (Leave Store)" << endl;
		cin >> temp;
		choice = stoi(temp);
		switch(choice)
		{
			case 1: 
				{
					float startPrice = 40;
					float price = startPrice + (startPrice/4 * storeNum);
					cout << "\tThere are two oxen in a yoke, each yoke is $" << price << endl;
					cout << "\tHow many yokes would you like to buy?" << endl;
					cin >> temp;
					choice = stoi(temp);
					p.payMoney(price * choice);

					p.addOxen(choice * 2);
					cout << "You bought " << choice << " yokes of oxen for $" << price * choice << "!" << endl;
					cout << "You now have " << p.getOxen() << " oxen." << endl;
				}
				break;
			case 2:
				{
					float startPrice = .5;
					float price = startPrice + (startPrice/4 * storeNum);
					cout << "Each pound of food costs " << price << "$." << endl;
					cout << "How many pounds would you like to buy?" << endl;
					cin >> temp;
					choice = stoi(temp);
					p.payMoney(price * choice);

					p.addFood(choice);
					cout << "You bought " << choice << " pounds of food for $" << price * choice << "!" << endl;
					cout << "You now have " << p.getFood() << " pounds of food." << endl;
				}
				break;
			case 3:
				{
					float startPrice = 2;
					float price = startPrice + (startPrice/4 * storeNum);
					cout << "A box of 20 bullets costs $" << price << endl;
					cout << "How many boxes would you like?" << endl;
					cin >> temp;
					choice = stoi(temp);
					p.payMoney(price * choice);

					p.addBullets(choice * 20);
					cout << "You bought " << choice << " boxes of bullets for $" << price * choice << "!" << endl;
					cout << "You now have " << p.getBullets() << " bullets." << endl;
				}
				break;
			case 4:
				{
					cout << "Would you like to buy: " << endl;
					cout << "\t1. Wagon wheels?" << endl;
					cout << "\t2. Wagon axles?" << endl;
					cout << "\t3. Wagon tongues?" << endl;
					cout << "\t4. Medical kits?" << endl;
					cin >> temp;
					choice = stoi(temp);
					switch(choice)
					{
						case 1:
						{
							float startPrice = 20;
							float price = startPrice + (startPrice/4 * storeNum);
							cout << "A wagon wheel costs $" << price << endl;
							cout << "How many would you like?" << endl;
							cin >> temp;
							choice = stoi(temp);
							p.payMoney(price * choice);
							cout << "You bought " << choice << " wagon wheels for $" << price* choice << "!" << endl;
							p.addExtraWheels(choice);
						}
							break;
						case 2:
						{
							float startPrice = 20;
							float price = startPrice + (startPrice/4 * storeNum);
							cout << "A wagon axle costs $" << price << endl;
							cout << "How many would you like?" << endl;
							cin >> temp;
							choice = stoi(temp);
							p.payMoney(price * choice);
							cout << "You bought " << choice << " wagon axles for $" << price* choice << "!" << endl;
							p.addExtraAxles(choice);
						}
							break;
						case 3:
						{
							float startPrice = 20;
							float price = startPrice + (startPrice/4 * storeNum);
							cout << "A wagon tongue costs $" << price << endl;
							cout << "How many would you like?" << endl;
							cin >> temp;
							choice = stoi(temp);
							p.payMoney(price * choice);
							cout << "You bought " << choice << " wagon tongues for $" << price* choice << "!" << endl;
							p.addExtraTongues(choice);
						}
							break;
						case 4:
						{
							float startPrice = 25;
							float price = startPrice + (startPrice/4 * storeNum);
							cout << "A medical kit costs $" << price << endl;
							cout << "How many would you like?" << endl;
							cin >> temp;
							choice = stoi(temp);
							p.payMoney(price * choice);
							cout << "You bought " << choice << " medical kits for $" << price* choice << "!" << endl;
						}
							break;
					}
				}
				break;
			case 5:
				{
					stillBuying = false;
				}
				break;
		}
	}
}

//returns an integer that tells the game what to do ater the turn.
/*
	Returns:
	4 - Player quit the game
	5 - Player beat the game
	6 - Player lost the game (unable to continue)
*/
int Game::doTurn()
{
	// cout << p.getExtraWheels() << endl;
	// cout << p.getExtraAxles()  << endl;
	// cout << p.getExtraTongues()<< endl;
	if (p.getOxen() <= 0)
	{
		cout << "You have no oxen!" << endl;
		return  6;
	}
	else if(!p.isAliveAt(0))
	{
		cout << "You are dead!" << endl;
		return 6;
	}

	//get the number that is alive.
	int numAlive = 0;
	for (int i = 0; i < 5; i++)
	{
		if (p.isAliveAt(i)) numAlive++;
	}

	int distToNextFort  = fortMiles[p.getLastFort()+1] - p.getMiles();
	int distToNextRiver = riverMiles[p.getLastRiver()+1] - p.getMiles();

	bool fortNext;
	fortNext = (distToNextFort < distToNextRiver);

	cout << "---------------------------------------------" << endl;
	cout << "The current date is: " << c.getDateString() << endl;
	cout << "You have travelled " << p.getMiles() << " miles." << endl;
	if (fortNext) cout << "The distance to " << fortNames[p.getLastFort()+1]   << " is " << distToNextFort  << " miles." << endl;
	else 		  cout << "The distance to " << riverNames[p.getLastRiver()+1] << " is " << distToNextRiver << " miles." << endl;
	cout << "You have " << p.getFood() << " pounds of food available." << endl;
	cout << "You have " << p.getBullets() << " bullets available" << endl;
	cout << "You have $" << p.getMoney() << " of cash on you." << endl;

	cout << "Would you like to:" << endl;
	cout << "\t1. Stop to rest?" << endl;
	cout << "\t2. Continue on the trail?" << endl;
	cout << "\t3. Hunt?" << endl;
	cout << "\t4. Quit?" << endl;
	string temp;
	cin >> temp;
	int choice = stoi(temp);
	switch(choice)
	{
		case 1:
		{
			cout << "How many days would you like to rest?" << endl;
			cin >> temp;
			choice = stoi(temp);
			c.advance(choice);
			p.addFood(-1*(3*numAlive*choice));
		}
			break;
		case 2:
		{
			int dist = 70 + rand() %70;
			if (fortNext)
			{
				if (dist > distToNextFort)
				{
					p.advance(distToNextFort);
					p.setLastFort(p.getLastFort()+1);
					if (p.getLastFort() == 5) return 5;
				}
				else
				{
					p.advance(dist);
				}
			}
			else
			{
				if (dist > distToNextRiver)
				{
					p.advance(distToNextRiver);
					p.setLastRiver(p.getLastRiver()+1);
				}
				else
				{
					p.advance(dist);
				}
			}
			p.addFood(-1*(3*14*numAlive));
			c.advance(14);
		}
			break;
		case 3:
		{
			cout << "You are now hunting..." << endl;
		}
			break;
		case 4:
		{
			return 4;
		}
			break;
	}


	//RAIDER ATTACK PROBABILITY forumal implementation
	float numerator = (p.getMiles()/100-4)*(p.getMiles()/100-4) + 72;
	float denominator = (p.getMiles()/100-4)*(p.getMiles()/100-4) + 12;

	float prob = ((numerator/denominator) - 1)/10;
	if (rand()%1000 < prob * 1000)
	{
		cout << "RAIDERS" << endl;
	}
	//MISFORTUNE probability
	if (rand() %10 <= 3)
	{
		//List of random misfortunes
		//1. party member gets sick
		//	 choose to Use medical kit, rest or press on.
		//	 chance of dying are   50%, 30%     70%
		//2. an oxen dies
		//	 if all oxen are dead, the party cannot continue
		//3. one part of the wagon breaks...
		//	 If the part is not repaired, the party cannot continue

		//lol
		cout << "Oh no!" << endl;
		int m = rand() % 3;
		switch(m)
		{
			//party member gets sick
			case 0:
			{
				int person = rand() % 5;
				cout << p.getNameAt(person) << " is sick!" << endl;
			}
				break;
			case 1:
			{
				cout << "One of your oxen died!" << endl;
				p.addOxen(-1);
				cout << "You now have " << p.getOxen() << " oxen." << endl;
				cout << "If you run out of oxen, you will be unable to travel." << endl;
			}
			case 2:
			{
				int part = rand()%3;
				switch(part)
				{
					//wheel
					case 0: 
					{ 
						cout << "One of your wagon's wheels broke!" << endl;
						if (p.getExtraWheels() > 0)
						{
							cout << "You used 1 spare wheel to repair it." << endl;
							p.addExtraWheels(-1);
						}
						else
						{
							cout << "You have no spare wheels, you cannot repair it." << endl;
							return 6;
						}
					} break;
					case 1: 
					{ 
						cout << "One of your wagon's axles broke!" << endl; 
						if (p.getExtraAxles() > 0) 
						{
							cout << "You used one spare axle to repair it." << endl;
							p.addExtraWheels(-1);
						}
						else
						{
							cout << "You have no spare axles, you cannot repair it." << endl;
							return 6;
						}
					} break;
					case 2: 
					{ 
						cout << "Your wagon's tongue broke!" << endl; 
						if (p.getExtraTongues() > 0) 
						{
							cout << "You used one spare tongue to repair it." << endl;
							p.addExtraTongues(-1);
						}
						else
						{
							cout << "You have no spare tongues, you cannot repair it." << endl;
							return 6;
						}
					} break;
				}
			}
		}
	}
}