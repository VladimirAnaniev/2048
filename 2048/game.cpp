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
int score = 0;
int board[SIZE][SIZE];
bool playing = true;

void gameOver()
{
	playing = false;
	cout << "Game Over!";
}

bool areThereFreeCells()
{
	//Search for free cells and return true if available
	for (int r = 0; r < SIZE; r++)
	{
		for (int c = 0; c < SIZE; c++)
		{
			if (board[r][c] == 0) return true;
		}
	}

	//No free cells found
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

void printCell(int num)
{
	int spaces = 6, number = num;
	if (num == 0) spaces--;
	while(num)
	{
		//Count the digits of the number and remove spaces
		num /= 10;
		spaces--;
	}

	
	for(int i=0;i<spaces;i++)
	{
		//print the number in the center of the cell
		if (i == spaces / 2) {
			if(number) cout << number;
			//Use a dot when the number is 0
			else cout << ".";
		}
		//Print spaces in the sides
		else cout << ' ';
	}
}

void printBoard()
{
	//Clear the console
	system("cls");

	//Print score
	cout << "Score: " << score << endl;

	cout << endl;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			//Print each number with spaces for alignment
			printCell(board[i][j]);
		}
		//Empty rows for alignment.
		cout << endl << endl << endl;
	}

	cout << "Use Arrow Keys to move." << endl << " Press Esc to quit";
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
		//Numbers are equal, update score, save the sum in the new cell and make old cell 0.
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