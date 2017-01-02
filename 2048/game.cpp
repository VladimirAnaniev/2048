#include <iostream>
#include <conio.h>

using namespace std;

#define SIZE 4
#define DOWN 'D'
#define UP 'U'
#define LEFT 'L'
#define RIGHT 'R'
int board[SIZE][SIZE];
bool playing = true;

bool areThereFreeCells()
{
	for(int r=0;r<SIZE;r++)
	{
		for(int c=0;c<SIZE;c++)
		{
			if (board[r][c] == 0) return true;
		}
	}
	return false;
}

void addRandom()
{
	int row, col;

	if (!areThereFreeCells())
	{
		playing = false;
		return;
	}
		

	do
	{
		row = rand() % SIZE;
		col = rand() % SIZE;
	} while (board[row][col] != 0);

	board[row][col] = 2;
}

void initializeBoard()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			board[i][j] = 0;
		}
	}

	addRandom();
	addRandom();
}

void printBoard()
{
	system("cls");

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

int getFreeCell(int n, char direction)
{
	if(direction == UP)
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (board[i][n] == 0) return i;
		}
	}
	else if (direction == DOWN)
	{
		for (int i = SIZE-1; i >= 0; i--)
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
	if(dir==UP)
	{
		for (int i = row-1; i>=0; i--)
		{
			if (board[i][col] != 0) return i;
		}
	}
	else if (dir == DOWN)
	{
		for (int i = row+1; i < SIZE; i++)
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

void makeAddition(int row, int col, int n, char dir)
{
	if (n == -1) return;

	if (dir == UP || dir == DOWN)
	{
		if (board[row][col] == board[n][col])
		{
			board[n][col] = 2 * board[row][col];
			board[row][col] = 0;
		}
	}
	else
	{
		if (board[row][col] == board[row][n])
		{
			board[row][n] = 2 * board[row][col];
			board[row][col] = 0;
		}
	}
}

void makeRelocation(int row, int col, char direction)
{


	if (board[row][col] != 0)
	{
		int temp = board[row][col];
		board[row][col] = 0;

		if(direction==UP||direction==DOWN)
		{
			int free = getFreeCell(col, direction);
			if(free != -1)
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

void makeMove(int n, char direction)
{
	if(direction == UP)
	{
		for (int row = 0; row < SIZE; row++)
		{
			int fullCandidate = getFullCell(row, n, direction);
			makeAddition(row, n, fullCandidate, direction);

			makeRelocation(row, n, direction);
		}
	}
	else if (direction == DOWN)
	{
		for (int row = SIZE - 1; row >= 0; row--)
		{
			int fullCandidate = getFullCell(row, n, direction);
			makeAddition(row, n, fullCandidate, direction);

			makeRelocation(row, n, direction);
		}
	}
	else if (direction == LEFT)
	{
		for (int col = 0; col < SIZE; col++)
		{
			int fullCandidate = getFullCell(n, col, direction);
			makeAddition(n, col, fullCandidate, direction);

			makeRelocation(n, col, direction);
		}
	}
	else if (direction == RIGHT)
	{
		for (int col = SIZE-1; col >= 0; col--)
		{
			int fullCandidate = getFullCell(n, col, direction);
			makeAddition(n, col, fullCandidate, direction);

			makeRelocation(n, col, direction);
		}
	}
}

void changeState(char direction)
{
	for (int i = 0; i < SIZE; i++)
	{
		makeMove(i, direction);
	}

	addRandom();
}

int main()
{
	initializeBoard();
	int c;

	while(playing)
	{
		printBoard();
		_getch();

		switch(_getch())
		{
		case 72: changeState(UP); break;
		case 80: changeState(DOWN); break;
		case 75: changeState(LEFT); break;
		case 77: changeState(RIGHT); break;
		case 27: playing = false; break;
		}
	}

	return 0;
}