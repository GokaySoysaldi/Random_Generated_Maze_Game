#include <iostream>            
#include <conio.h>
#include <fstream>
#include <ctime>                                 
#include <stdlib.h>
#include <windows.h>

using namespace std;
                            //Free Path:0  Wall:1  Monster:S  Gold:A  Location:X
							//Use ARROW keys to play

int main()
{
	//Maze Specs
	int mazeW;
	int mazeH;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << "Type Maze Height And Width With Space:" << endl;
	cin >> mazeH >> mazeW;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	//Maze dimension size check
	while (mazeH < 10 || mazeH>100 || mazeW < 10 || mazeW>100)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout << endl << "Minimum size is 10x10 and maximum size is 100x100 !" << endl;
		cout << "Type maze height and width with space:" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		cin >> mazeH >> mazeW;
	}

	//Maze char array declaration
	char maze[100][100];

	//Maze initialization with free spaces
	for (int i = 0; i < mazeH; i++)
	{
		for (int j = 0; j < mazeW; j++)
		{
			maze[i][j] = '0';
		}
	}

	//Get current time in int64 format
	time_t _time;
	time(&_time);

	//Create seed for the current time
	srand(_time);

	//Create walls with 1/6 probability
	for (int i = 0; i < mazeH; i++)
	{
		for (int j = 0; j < mazeW; j++)
		{
			if (!(i == 0 && j == 0) && !(i == mazeH - 1 && j == mazeW - 1))
			{
				if (rand() % 6 < 1)
				{
					maze[i][j] = '1';

				}
			}
		}
	}

	//Create monsters with 1/13 probability
	for (int i = 0; i < mazeH; i++)
	{
		for (int j = 0; j < mazeW; j++)
		{
			if (!(i == 0 && j == 0) && !(i == mazeH - 1 && j == mazeW - 1))
			{
				if (rand() % 13 < 1)
				{

					maze[i][j] = 'S';

				}
			}
		}
	}

	//Create golds with 1/5 probability
	for (int i = 0; i < mazeH; i++)
	{
		for (int j = 0; j < mazeW; j++)
		{
			if (!(i == 0 && j == 0) && !(i == mazeH - 1 && j == mazeW - 1))
			{
				if (rand() % 5 < 1)
				{
					maze[i][j] = 'A';
				}
			}
		}
	}

	//Create copy of the maze to restore
	char backup[100][100];

	for (int i = 0; i < mazeH; i++)
	{
		for (int j = 0; j < mazeW; j++)
		{
			backup[i][j] = maze[i][j];
		}
	}

	//Define variables
	int keyPress;
	int iLocation;
	int jLocation;
	int gold;

	//Reset label is set to go back when game resets
gamereset:

	//Restore the maze
	for (int i = 0; i < mazeH; i++)
	{
		for (int j = 0; j < mazeW; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			maze[i][j] = backup[i][j];
		}
	}

	//Variable declaration
	iLocation = 0;
	jLocation = 0;
	gold = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	maze[0][0] = 'X';
	int exitpath[1000][2];
	char moves[1000];
	int counter = 0;


	//Game loop for arrow controls
	while (true)
	{
		//Clear console
		system("cls");
		//Print maze
		for (int i = 0; i < mazeH; i++)
		{
			for (int j = 0; j < mazeW; j++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << maze[i][j] << "  ";
			}
			cout << endl;
		}

		//Get the pressed key by Getche. Used twice because arrow keys throws an error in the first call, then returns the arrow code
		keyPress = _getche();
		keyPress = _getche();

		//Determine the key and set index up for the new location
		switch (keyPress) 
		{
		case 72: //key up
			iLocation--;
			break;
		case 80: // key down
			iLocation++;
			break;
		case 77: // key right
			jLocation++;
			break;
		case 75: // key left
			jLocation--;
			break;
		}

		//If monster is in the moved cell, reset game
		if (maze[iLocation][jLocation] == 'S')
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			cout << "Press Up Arrow To Try Again" << endl;
			keyPress = _getche();
			keyPress = _getche();
			goto gamereset;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}

		//If there is gold in moved cell, increment gold counter
		if (maze[iLocation][jLocation] == 'A')
		{
			gold++;
		}

		//If maze borders exceeded or there is a wall, then restore previous location
		//If moved cell available, then clear the previous cell
		if (iLocation<0 || jLocation<0 || iLocation>mazeH - 1 || jLocation>mazeW - 1 || maze[iLocation][jLocation] == '1') {
			switch (keyPress)
			{
			case 72:
				//key up
				iLocation++;
				break;
			case 80:
				// key down
				iLocation--;
				break;
			case 77:
				// key right
				jLocation--;
				break;
			case 75:
				// key left
				jLocation++;
				break;
			}
		}
		else
		{
			switch (keyPress)
			{
			case 72: //key up
				maze[iLocation + 1][jLocation] = '0';
				moves[counter] = 'u';
				break;
			case 80: // key down
				maze[iLocation - 1][jLocation] = '0';
				moves[counter] = 'd';
				break;
			case 77: // key right
				maze[iLocation][jLocation - 1] = '0';
				moves[counter] = 'r';
				break;
			case 75: // key left
				maze[iLocation][jLocation + 1] = '0';
				moves[counter] = 'l';
				break;
			}
			counter++;
		}

		//Change moved location as current location
		maze[iLocation][jLocation] = 'X';

		//Set exit path
		exitpath[counter][0] = iLocation;
		exitpath[counter][1] = jLocation;

		//If exit cell reached, finish the after printing messages
		if (iLocation == mazeH - 1 && jLocation == mazeW - 1)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			cout << endl << "Congrats, You Won !" << endl;
			cout << "Total Gold: " << gold << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

			//Output stream for writing files
			ofstream gameStatStream;

			//Write path to Exit Path.txt
			gameStatStream.open("Exit Path.txt", ios::app);
			gameStatStream << "x y\n";
			gameStatStream << "- -\n";

			for (int i = 0; i < counter; i++)
			{
				if (i > 0)
				{
					gameStatStream << exitpath[i][0] << " " << exitpath[i + 1][1] << "\n";
				}
			}

			gameStatStream << "\n\n";
			gameStatStream.close();

			//Write moves to Moves.txt
			gameStatStream.open("Moves.txt", ios::app);
			gameStatStream << "Moves (r:Right l:Left u:Up d:Down)\n";

			for (int i = 0; i < counter; i++)
			{
				gameStatStream << moves[i] << "\n";
			}

			gameStatStream << "\n\n";
			gameStatStream.close();

			//Break the game loop
			break;
		}
	}

	system("pause");
}
