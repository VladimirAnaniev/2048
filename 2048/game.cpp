#include <iostream>
#include <conio.h>

using namespace std;

//Define constants
#define SIZE 4
#define DOWN 'D'
#define UP 'U'
#define LEFT 'L'
#define RIGHT 'R'

//Global variables
int board[SIZE][SIZE];
bool playing = true;

void gameOver()
{
	//End game
	playing = false;
	cout << "Game Over!";
}

bool areThereFreeCells()
{
	//Check if there are any free cells.
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

	if (!areThereFreeCells())
	{
		//If no free chells are available, end the game.
		gameOver();
		return;
	}
		
	do
	{
		//Generate random colnum and row number for the new cell
		row = rand() % SIZE;
		col = rand() % SIZE;
	} while (board[row][col] != 0);

	//Add the cell to the board.
	board[row][col] = 2;
}

void initializeBoard()
{
	//Make all cells on the board zeros.
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			board[i][j] = 0;
		}
	}

	//Add two initial numbers.
	addRandom();
	addRandom();
}

void printSpaces(int num)
{
	int count = 0;
	if (num == 0) count++;
	while(num)
	{
		//Count the digits of the number
		num /= 10;
		count++;
	}

	
	for(int i=count;i<6;i++)
	{
		//Print spaces to align all cells
		cout << ' ';
	}
}

void printBoard()
{
	//Clear the console
	system("cls");

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			//Print each number with spaces for alignment
			cout << board[i][j];
			printSpaces(board[i][j]);
		}
		//Empty rows for alignment.
		cout << endl << endl << endl;
	}

}

int getFreeCell(int n, char direction)
{
	//Return the index of the first free cell in the given direction.
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

	//Return -1 when no free cells are found
	return -1;
}

int getFullCell(int row, int col, char dir)
{
	//Return the index of the nearest full cell in the given direction.
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

	//Return -1 if no full cells are found.
	return -1;
}

void makeAddition(int row, int col, int n, char dir)
{
	//Add two given numbers if they have the same value
	if (dir == UP || dir == DOWN)
	{
		if (board[row][col] == board[n][col])
		{
			board[n][col] = 2 * board[row][col];
			board[row][col] = 0;
		}
	}
	else if (dir == LEFT || dir == RIGHT)
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
	//Move a given number in the given direction.
	if (board[row][col] != 0)
	{
		//save the current value and make the cell 0.
		int temp = board[row][col];
		board[row][col] = 0;

		//Set the value of the first free cell to the previously saved value;
		if (direction == UP || direction == DOWN)
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

bool makeMove(int n, char direction)
{
	//Make all operations on a given row/col
	int arrBefore[4], arrAfter[4], row=0, col=0;

	for (int i = 0; i < SIZE; i++)
	{
		//First setup the variables depending on the direction
		if (direction == UP)
		{
			row = i;
			col = n;
		}
		else if(direction == DOWN)
		{
			row = SIZE - 1 - i;
			col = n;
		}
		else if(direction == LEFT)
		{
			row = n;
			col = i;
		}
		else if(direction == RIGHT)
		{
			row = n;
			col = SIZE-1-i;
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

	//Check for changes.
	for (int i = 0; i < SIZE; i++)
	{
		//there was a difference => a change was made => return true
		if(arrBefore[i] != arrAfter[i]) return true;
	}

	//Return false for no difference
	return false;
}

void changeState(char direction)
{
	//Make changes for all rows/cols in a given direction
	bool changed = false;

	for (int i = 0; i < SIZE; i++)
	{
		//Make all operations on current row/col and get bool value if changes were made
		bool current = makeMove(i, direction);

		//If changes were made on current row/col, change helper
		if (current) changed = true;
	}

	//Only add a new element if changes were made
	if (changed) addRandom();
	//If no changes were made, make an alert sound
	else cout << '\a';
}

int main()
{
	//Initialize the game
	initializeBoard();

	//Game loop
	while(playing)
	{
		//Self-explanatory..
		printBoard();

		//When arrow key is pressed, _getch is called twice
		//Escape the first one
		_getch();

		//Make moves depending on the pressed keyboard key
		switch(_getch())
		{
			//UP arrow key
			case 72: changeState(UP); break;
			//DOWN arrow key
			case 80: changeState(DOWN); break;
			//LEFT arrow key
			case 75: changeState(LEFT); break;
			//RIGHT arrow key
			case 77: changeState(RIGHT); break;
			//Esc
			case 27: gameOver(); break;
		}
	}

	return 0;
}