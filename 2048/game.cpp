#include <iostream>
#include <conio.h>

using namespace std;

//Define constants
//#define SIZE 4
#define DOWN 'D'
#define UP 'U'
#define LEFT 'L'
#define RIGHT 'R'

//Global variables
int SIZE = 4;
int score = 0;
int** board;
bool playing = true;

void clear()
{
	system("cls");
}

bool areThereFreeCells()
{
	for (int r = 0; r < SIZE; r++)
	{
		for (int c = 0; c < SIZE; c++)
		{
			if (board[r][c] == 0) return true;
		}
	}

	return false;
}

void addRandom()
{
	int row, col;

	do
	{
		row = rand() % SIZE;
		col = rand() % SIZE;
	} while (board[row][col] != 0);

	board[row][col] = 2;
}
void initializeBoard()
{
	board = new int*[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		int* curr = new int[SIZE];
		for (int j = 0; j < SIZE; j++)
		{
			curr[j] = 0;
		}
		board[i] = curr;
	}

	addRandom();
	addRandom();

	score = 0;
}

int initSettings()
{
	system("cls");
	char n;

	cout <<endl<< "Enter a number between 3 and 9 for size of the board." << endl;

	while(true)
	{
		cin >> n;
		if (n >= '3' && n <= '9') return n-'0';

		cout << endl << "Try again" << endl;
	}
}

void deleteBoard()
{
	for(int i=0;i<SIZE;i++)
	{
		delete[] board[i];
	}

	delete[] board;
}

void gameOver()
{
	deleteBoard();

	clear();
	cout << endl << endl << "----- Game Over! -----";
	cout << endl << endl << "--Your score: " <<score<<" --"<< endl << endl;
	cout << "type 'r' to try again and 'q' to quit." << endl;

	char input;
	while(true)
	{
		cin >> input;
		if (input == 'q') {
			playing = false;
			break;
		}
		if (input == 'r') {
			SIZE = initSettings();
			initializeBoard();
			break;
		}

		cout <<endl<< "Try again!" << endl;
	}

	
}

void printCell(int num)
{
	int spaces = 6, number = num;
	if (num == 0) spaces--;
	while (num)
	{
		num /= 10;
		spaces--;
	}


	for (int i = 0; i < spaces; i++)
	{
		//print the number in the center of the cell
		if (i == spaces / 2)
		{
			if (number) cout << number;
			//Use a dot when the number is 0
			else cout << ".";
		}
		//Print spaces in the sides
		else cout << ' ';
	}
}

void printBoard()
{
	clear();

	//Print score
	cout << "Score: " << score << endl;

	cout << endl;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			printCell(board[i][j]);
		}
		cout << endl << endl << endl;
	}

	cout << "Use Arrow Keys to move." << endl << " Press Esc to quit";
}

int getFreeCell(int n, char direction)
{
	//Return the index of the first free cell in the given direction.
	if (direction == UP)
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (board[i][n] == 0) return i;
		}
	}
	else if (direction == DOWN)
	{
		for (int i = SIZE - 1; i >= 0; i--)
		{
			if (board[i][n] == 0) return i;
		}
	}
	else if (direction == LEFT)
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (board[n][i] == 0) return i;
		}
	}
	else if (direction == RIGHT)
	{
		for (int i = SIZE - 1; i >= 0; i--)
		{
			if (board[n][i] == 0) return i;
		}
	}

	return -1;
}

int getFullCell(int row, int col, char dir)
{
	//Return the index of the nearest full cell in the given direction.
	if (dir == UP)
	{
		for (int i = row - 1; i >= 0; i--)
		{
			if (board[i][col] != 0) return i;
		}
	}
	else if (dir == DOWN)
	{
		for (int i = row + 1; i < SIZE; i++)
		{
			if (board[i][col] != 0) return i;
		}
	}
	else if (dir == LEFT)
	{
		for (int i = col - 1; i >= 0; i--)
		{
			if (board[row][i] != 0) return i;
		}
	}
	else if (dir == RIGHT)
	{
		for (int i = col + 1; i < SIZE; i++)
		{
			if (board[row][i] != 0) return i;
		}
	}

	return -1;
}

void updateScore(int num)
{
	score += num;
}

void makeAddition(int row, int col, int n, char dir)
{
	int newRow = row, newCol = col;

	//Change variables depending on direction
	if (dir == UP || dir == DOWN) newRow = n;
	else if (dir == LEFT || dir == RIGHT) newCol = n;

	if (board[row][col] == board[newRow][newCol])
	{
		int sum = 2 * board[row][col];
		updateScore(sum);
		board[newRow][newCol] = sum;
		board[row][col] = 0;
	}
}

void makeRelocation(int row, int col, char direction)
{
	//Move a given number in the given direction.
	if (board[row][col] != 0)
	{
		int temp = board[row][col];
		board[row][col] = 0;

		if (direction == UP || direction == DOWN)
		{
			int free = getFreeCell(col, direction);
			if (free != -1)
				board[free][col] = temp;
		}
		else
		{
			int free = getFreeCell(row, direction);
			if (free != -1)
				board[row][free] = temp;
		}
	}
}

bool makeMove(int n, char direction)
{
	//Make all operations on a given row/col

	int* arrBefore = new int[SIZE];
	int* arrAfter = new int[SIZE];
	int row = 0, col = 0;

	for (int i = 0; i < SIZE; i++)
	{
		if (direction == UP)
		{
			row = i;
			col = n;
		}
		else if (direction == DOWN)
		{
			row = SIZE - 1 - i;
			col = n;
		}
		else if (direction == LEFT)
		{
			row = n;
			col = i;
		}
		else if (direction == RIGHT)
		{
			row = n;
			col = SIZE - 1 - i;
		}

		//Fill array to check if changes were made
		arrBefore[i] = board[row][col];

		//Make additions if possible
		int fullCandidate = getFullCell(row, col, direction);
		if (fullCandidate != -1)
			makeAddition(row, col, fullCandidate, direction);

		//Move cells in the given direction
		makeRelocation(row, col, direction);

		//Fill array to check if changes were made
		arrAfter[i] = board[row][col];
	}

	bool returnVal = false;
	//Check for changes.
	for (int i = 0; i < SIZE; i++)
	{
		if (arrBefore[i] != arrAfter[i]) returnVal = true;
	}

	delete[] arrBefore, arrAfter;

	return returnVal;
}

void changeState(char direction)
{
	//Make changes for all rows/cols in a given direction
	bool changed = false;

	for (int i = 0; i < SIZE; i++)
	{
		bool current = makeMove(i, direction);

		if (current) changed = true;
	}

	//Only add a new element if changes were made
	if (changed) addRandom();
	//If no changes were made, make an alert sound
	else cout << '\a';
	//If no free cells available and no change was made end the game
	if (!areThereFreeCells() && !changed) gameOver();
}

int main()
{
	SIZE = initSettings();
	initializeBoard();

	while (playing)
	{
		printBoard();

		//When arrow key is pressed, _getch is called twice
		//Escape the first one
		_getch();

		//Make moves depending on the pressed keyboard key
		switch (_getch())
		{
		case 72: changeState(UP); //UP arrow key
			break;
		case 80: changeState(DOWN); //DOWN arrow key
			break;
		case 75: changeState(LEFT); //LEFT arrow key
			break;
		case 77: changeState(RIGHT); //RIGHT arrow key
			break;
		case 27: gameOver(); //Esc
			break;
		}
	}

	return 0;
}
