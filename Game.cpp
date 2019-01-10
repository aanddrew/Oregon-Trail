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
							p.addMedicalKits(choice);
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
	string temp;
	int choice;
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
	else if(p.getFood() < 0)
	{
		cout << "You ran out of food!" << endl;
		return 6;
	}

	//get the number that is alive.
	int numAlive = 0;
	for (int i = 0; i < 5; i++)
	{
		if (p.isAliveAt(i)) numAlive++;
	}

	bool medicalKitUsed[5] = {false, false, false, false, false};

	for (int i = 0; i < 5; i++)
	{
		if (p.isSickAt(i) && p.isAliveAt(i))
		{
			cout << p.getNameAt(i) << " is sick!" << endl;
			if (p.getMedicalKits() > 0)
			{
				cout << "Would you like to use a medical kit? (Y/N)" << endl;
				cout << "If you continue withOUT using one they have a 70 percent chance of death." << endl;
				cout << "If you continue WITH using a medical kit they have a 50 percent chance of death." << endl;
				cout << "If you rest without using one they have a 30 percent chance of death." << endl;
				cin >> temp;
				if (temp == "Y" || temp == "y")
				{
					p.addMedicalKits(-1);
					medicalKitUsed[i] = true;
				}
			}
			else
			{
				cout << "You have no medical kits." << endl;
				cout << "If you rest they have a 30 percent chance of death." << endl;
				cout << "If you continue they have a 70 percent chance of death." << endl;
			}
		}
	}

	int distToNextFort  = fortMiles[p.getLastFort()+1] - p.getMiles();
	int distToNextRiver = riverMiles[p.getLastRiver()+1] - p.getMiles();

	int distToLastFort  = fortMiles[p.getLastFort()] - p.getMiles();
	int distToLastRiver = riverMiles[p.getLastRiver()] - p.getMiles();

	bool fortNext;
	fortNext = (distToNextFort < distToNextRiver);

	cout << "---------------------------------------------" << endl;
	cout << "The current date is: " << c.getDateString() << endl;
	cout << "You have travelled " << p.getMiles() << " miles." << endl;
	if (distToLastFort == 0)
	{
		cout << "You are currently in " << fortNames[p.getLastFort()] << endl;
	}
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

	if (distToLastFort == 0)
	{
		cout << "\t5. Shop?" << endl;
	}

	cin >> temp;
	choice = stoi(temp);
	switch(choice)
	{
		case 1:
		{
			cout << "How many days would you like to rest?" << endl;
			cin >> temp;
			choice = stoi(temp);
			c.advance(choice);
			p.addFood(-1*(3*numAlive*choice));

			for (int i = 0; i < 5; i++)
			{
				if (p.isSickAt(i) && p.isAliveAt(i))
				{
					if (rand() %100 < 30)
					{
						cout << p.getNameAt(i) << " is dead!" << endl;
						p.kill(i);
					}
					else
					{
						cout << p.getNameAt(i) << " is no longer sick." << endl;
						p.cure(i);
					}
				}
			}
		}
			break;
		case 2:
		{
			int dist = 70 + rand() %70;
			//they must cross the river if they reach it
			if (distToLastRiver == 0)
			{
				cout << "You must cross the " << riverNames[p.getLastRiver()] << "." << endl;
			}

			//deciding how to move them if they are close to a river or fort...
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

			//checking if sick players die
			for (int i = 0; i < 5; i++)
			{
				if (p.isSickAt(i) && p.isAliveAt(i))
				{
					if (medicalKitUsed[i])
					{
						if (rand() %100 < 50)
						{
							cout << p.getNameAt(i) << " is dead!" << endl;
							p.kill(i);
						}
						else
						{
							cout << p.getNameAt(i) << " is no longer sick." << endl;
							p.cure(i);
						}
					}
					else
					{
						if (rand() %100 < 70)
						{
							cout << p.getNameAt(i) << " is dead!" << endl;
							p.kill(i);
						}
						else
						{
							cout << p.getNameAt(i) << " is no longer sick." << endl;
							p.cure(i);
						}
					}
				}
			}

			//continuing takes two weeks, during which each alive player consumes 3 pounds of food
			p.addFood(-1*(3*14*numAlive));
			//calendar advancing two weeks
			c.advance(14);
		}
			break;
		case 3:
		{
			//placeholder
			// cout << "You are now hunting..." << endl;

			//this is the list of probabilities for each animal
			int probs[5] = {50, 25, 15, 7, 5};
			string animalNames[5];
			animalNames[0] = "rabbit";
			animalNames[1] = "fox";
			animalNames[2] = "deer";
			animalNames[3] = "bear";
			animalNames[4] = "moose";
			int numBullets[5] = {10, 8, 5, 10, 12};
			int pounds[10] = {2,2,5,5,30,55,100,350,300,600};


			int totalFood = 0;
			bool animalFound = false;
			for (int i = 0; i < 5; i++)
			{
				int val = rand() % 100;
				if (val < probs[i])
				{
					animalFound = true;
					cout << "You found a ";
					cout << animalNames[i] << endl;
					cout << "Do you want to hunt it? (Y/N)" << endl;
					cin >> temp;
					if (temp == "Y" || temp == "y")
					{
						if (p.getBullets() >= numBullets[i])
						{
							if (puzzle())
							{
								// cout << "Congratulations! You just got a " << animalNames[i] << "!" << endl;
								cout << "Nice! You just got a " << animalNames[i] << "!" << endl;
								cout << "You used " << numBullets[i] << " bullets." << endl;
								p.addBullets(-1*numBullets[i]);
								int range = pounds[(i*2)+1] - pounds[i*2];
								cout << range << endl;
								int received;
								if (range != 0) received = pounds[i*2] + (rand() % range);
								else received = pounds[i*2];
								cout << "You received " << received << " pounds of food!" << endl;
								totalFood += received;
								cout << "You have earned " << totalFood << " pounds of food from this hunting day so far!" << endl;
							}
							else
							{
								cout << "You lost the " << animalNames[i] << "... Better luck next time!" << endl;
							}
						}
						else
						{
							cout << "You don't have enough bullets, sorry!" << endl;
						}
					}
				}
			}

			if (animalFound)
			{
				if (totalFood == 0)
				{
					cout << "Ouch... You found something but didnt catch it." << endl;
				}
				else
				{
					cout << "You found " << totalFood << " pounds of food!" << endl;
				}
				p.addFood(totalFood);
			}
			else
			{
				cout << "You didn't find any animals hunting. :(" << endl;
				cout << "Better luck next time!" << endl;
			}

			c.advance(1);
			p.addFood(-1*(3*numAlive));
		}
			break;
		case 4:
		{
			return 4;
		}
			break;
		case 5:
		{
			if (distToLastFort == 0)
			{
				shop(p.getLastFort() + 1);
			}
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
				if (person == 0)
				{
					cout << "YOU (The party leader) are sick!" << endl;
					cout << "If you die you will lose!" << endl;
				}
				else cout << p.getNameAt(person) << " is sick!" << endl;
				p.makeSick(person);
			}
				break;
			case 1:
			{
				cout << "One of your oxen died!" << endl;
				p.addOxen(-1);
				cout << "You now have " << p.getOxen() << " oxen." << endl;
				cout << "If you run out of oxen, you will be unable to travel." << endl;
			}
				break;
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

bool Game::puzzle()
{
	cout << "You have 3 tries to guess the correct number between 1 and 10." << endl;
	cout << "The number does not change so don't put the same number twice." << endl;
	int target = rand() % 10 +1;
	string temp;
	int choice;
	for (int i = 0; i < 3; i++)
	{
		cout << i + 1 << ". ";
		cin >> temp;
		choice = stoi(temp);
		if (choice == target)
		{
			cout << "Congratulations! You guessed the number correctly!" << endl;
			return true;
		}
	}
	cout << "You did not guess the number. :(" << endl;
	return false;
}