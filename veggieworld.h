#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <chrono>
#include <stdlib.h> 
#include <time.h>
#include <iomanip>
#include <fstream>
using namespace std;

const int WIDTH = 50; //size of the play area
const int HEIGHT = 25;
//optimal command prompt window size is 85 width 30 height
void gotoxy(int horizontal, int vertical) //This moves the objects by manipulating the x and y coordinates
{
	COORD coord;
	coord.X = horizontal;
	coord.Y = vertical;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ShowConsoleCursor(bool showFlag) //removes blinking cursor
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

class world //constructor establishes game area and runs game
{
private:
	void winMessage(double acc);
	void loseMessage();
public:
	world();
	//~world() {};
};

class veggie
{
public:
	veggie();
	~veggie() {};
	int x;
	bool Stopped = false;
	int y = 1;
	void Fall();
	void displayveggie();
};

veggie::veggie() //constructor that places the veggie in a random horizontal position
{
	x = (rand() % (WIDTH - 2)) + 1; //random number from 1 to width-1 so that it does not spawn inside the border
	gotoxy(x, 1);
	cout << "O";
}

void veggie::Fall()
{
	if (Stopped) //when the veggie has stopped, it will no longer move
	{
		y = HEIGHT + 4; //hides the veggie under the play area so that the points will not bug
		gotoxy(x, y);
		_putch(' ');
	}
	else
	{
		gotoxy(x, y); //if not stopped, it will continue to move down
		_putch(' ');
		y++;
	}
}

void veggie::displayveggie()//displays veggie at target coordinates
{
	gotoxy(x, y);
	cout << "O";
}

bool checkPoints(int veggieX, int veggieY, int plateX, int plateY)//checks if the veggie was caught by the character
{
	if ((plateX == veggieX || plateX + 1 == veggieX || plateX + 2 == veggieX || plateX + 3 == veggieX || plateX + 4 == veggieX) && (plateY + 1) == veggieY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool checkFail(int veggieY, int plateY)//checks if the character has missed the veggie
{
	if ((plateY + 1) == veggieY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void setBorder(int x, int y)//prints the border
{
	for (int r = 0; r < x; ++r)
	{
		gotoxy(r, 0);
		cout << '\xDB';
	}

	for (int b = 0; b < y; ++b)
	{
		gotoxy(0, b);
		cout << '\xDB';
	}

	for (int b = 0; b < y; ++b)
	{
		gotoxy(x, b);
		cout << '\xDB';
	}
}

void displayYouZoo(int x, int y) //displays youzoo at target location
{
	gotoxy(x - 1, y);
	cout << " _____ ";
	gotoxy(x, y + 1);
	cout << " \\o/ ";
	gotoxy(x, y + 2);
	cout << "  |  ";
	gotoxy(x, y + 3);
	cout << " / \\ ";
}
world::world()
{
	unsigned int points = 0;
	unsigned int exc = 0;
	unsigned int hits = 1, misses = 0, acc = 0;
	system("cls");
	ShowConsoleCursor(false);
	setBorder(WIDTH, HEIGHT); //creates the border according to length and height

	int x, y;
	x = 25; //initial position of the character
	y = 25;
	gotoxy(x, y);
	displayYouZoo(x, y);

	int i = 0; int k = 0;
	int leftpressed, rightpressed;
	veggie *veggieList[500];

	int veggieRespawn = 30;

	while (true)
	{
		if (i % veggieRespawn == 0)//changes the time between each new fruit
		{
			veggieList[k] = new veggie;
			k++;
		}
		leftpressed = 0;
		rightpressed = 0;

		using namespace std::chrono;

		high_resolution_clock::time_point t1 = high_resolution_clock::now(); //marks the time before the loop as t1
		while (1)
		{
			if (GetKeyState(VK_RIGHT) < 0)  //tests if the key is pressed or not. If not, it moves on
			{
				rightpressed = 1;
				if (GetKeyState(VK_SPACE) < 0)  // tests if the space bar is pressed at the same time
				{
					rightpressed = 2;
				}
			}
			if (GetKeyState(VK_LEFT) < 0)
			{
				leftpressed = 1;
				if (GetKeyState(VK_SPACE) < 0)
				{
					leftpressed = 2;
				}
			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now(); //sets t2 to the time at the end of each loop
			duration<double, std::milli> time_span = t2 - t1;//sets time_span as the time elapsed during the loop
			if (time_span.count() > 25) { break; }//when time elapsed in milliseconds has past a mark, loop will break 
		}

		if (leftpressed) //if the key was pressed within the previous loop, the character will move one time.
		{
			x--;
			if (x < 1)//stops the guy from moving out of the play area
			{
				x = 1;
			}
			displayYouZoo(x, y);
		}

		if (rightpressed)
		{
			x++;
			if (x > (WIDTH - 4)) //minus 4 to fit the size of the platform
			{
				x = (WIDTH - 4);
			}
			displayYouZoo(x, y);
		}

		if (leftpressed == 2) //if the space bar was pressed with the arrow key, the character will move twice as fast
		{
			for (int j = 0; j < 2; j++) //change # of loops to change speed increase
			{
				x--;
				if (x < 0)
				{
					x = 0;
				}
				displayYouZoo(x, y);
			}
		}

		if (rightpressed == 2) //moves to the right
		{
			for (int j = 0; j < 2; j++)
			{
				x++;
				if (x > 46) //limits movement to inside play area
				{
					x = 46;
				}
				displayYouZoo(x, y);
			}
		}

		for (int j = 0; j < k; j++)
		{
			veggieList[j]->Fall();
			if (checkPoints(veggieList[j]->x, veggieList[j]->y, x, y))//checks if the plate and the veggie are at the same place
			{
				gotoxy(veggieList[j]->x, veggieList[j]->y - 1);
				_putch('_');
				veggieList[j]->Stopped = true; //if the veggie was caught, it will stop
				veggieList[j]->Fall();
				points++;
				hits++;
			}
			if (checkFail(veggieList[j]->y, y))//checks if veggie has fallen past the character and will stop it from moving
			{
				veggieList[j]->Stopped = true;
				veggieList[j]->Fall();
				misses++;
			}
			else if (veggieList[j]->y > HEIGHT) {}
			else { veggieList[j]->displayveggie(); }//if not continue going down
		}
		
		i++;//counts the amount of loop cycles 

		try
		{
			if (points >= 50)
			{
				throw exc; //winning exception
			}
			else
			{
				gotoxy(WIDTH + 10, 0);
				cout << points << " Points"; //shows points
			}
			acc = ((hits / (hits + misses)) * 100); //calculates hit accuracy
			if (acc <= 25) 
			{
				throw 'c'; //losing exception
			}
			else
			{
				gotoxy(WIDTH + 10, 1);
				cout << "Accuracy: " << setprecision(2) << fixed << acc << "% "; //shows player accuracy
			}
		}
		catch (int exc)
		{
			winMessage(acc); //passes accuracy to winning function for storage into file
			break;
		}
		catch (char lose)
		{
			loseMessage();
			break;
		}
	}
}
void world::winMessage(double acc)//message that displays when you win the game
{
	string ranking;
	system("cls");
	cout << "Nice job, you win!" << endl;
	cout << "Enter your name: ";
	cin >> ranking;
	ofstream output; //stores user data in a text file in parent folder
	output.open("scores.txt", ios::app);
	output << ranking << ": " << acc << endl;
	output.close();
	cout << "Scores are saved in scores.txt in the parent folder!";
	cin.get();
	cin.ignore();
}

void world::loseMessage()//only winning scores are stored
{
	system("cls"); 
	cout << "Too bad, you lost! Try again next time!";
	cin.get();
	cin.ignore();
}

void game()
{
	system("color 0A"); //green text with black background
	cout << "Welcome to Catch the Vegetable!\n\nUse your left and right arrow keys to control YouZoo and catch the vegetables!\n\n";
	cout << "Use the spacebar to speed up your movement.\n\nCollect 50 veggies to win, but don't let your accuracy drop below 25%! Good Luck!\n\n";
	cout << "Press enter to begin!";
	cin.ignore();
	while (true)
	{
		world* w = new world; // allocates space for class object
		while(true)
		{ 
			system("cls");
			cout << "Would you like to play again? y/n\n"; //allows player to play again without closing and relaunching the program
			char c;
			cin >> c;
			if (c == 'y') //breaks loop if user wants to continue, allowing the class object to be deallocated then reallocated anew for a new session
			{
				break;
			}
			else if (c == 'n') //closes program if user does not want to play
			{
				return;
			}
			else
			{
				cout << "Invalid response, please try again.";  //input validation by looping question
				Sleep(1000);
			}
		}
		delete w; //deletes class object to prevent memory leakage
	}
		
}