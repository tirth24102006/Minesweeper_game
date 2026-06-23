#include <iostream>
#include <cstdlib> // For system()
#include <limits>  // For numeric_limits
#include <ctime>
#ifdef _WIN32
#include <windows.h> // For Sleep()
#else
#include <unistd.h> // For usleep()
#endif
using namespace std;

#define BEGINNER 0
#define INTERMEDIATE 1
#define ADVANCED 2
#define MAXSIDE 25
#define MAXMINES 99
#define MOVESIZE 526 // (25 * 25 - 99)

int SIDE;	    // side length of the board
int MINES;	    // number of mines on the board
int remainingMines; // mines left to flag

// ANSI color codes for terminal colors
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

// Clear screen command based on OS
void clearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

// Function to create a loading animation
void loadingAnimation(int duration)
{
	std::cout << "Loading ";
	for (int i = 0; i < 3; i++)
	{
		std::cout << ".";
		std::cout.flush();
#ifdef _WIN32
		Sleep(duration); // Sleep in milliseconds for Windows
#else
		usleep(duration * 1000); // Convert milliseconds to microseconds for Unix
#endif
	}
	clearScreen();
}

// Function to create a simple explosion animation
void explosionAnimation()
{
	clearScreen();
	const char *explosion[] = {
	    "   **   \n  ****  \n  ****  \n   **   ",
	    "  ****  \n ****** \n ****** \n  ****  ",
	    " ****** \n*\n*\n ****** ",
	    "  BOOM  \n  BOOM  \n  BOOM  \n  BOOM  "};

	for (int i = 0; i < 4; i++)
	{
		clearScreen();
		cout << COLOR_RED << explosion[i] << COLOR_RESET << endl;
#ifdef _WIN32
		Sleep(300);
#else
		usleep(300000);
#endif
	}
}

// Function to create a celebration animation for winning
void celebrationAnimation()
{
	clearScreen();
	const char *fireworks[] = {
	    "*   *   *   *\n  *   *   *  ",
	    "  *   *   *  \n*   *   *   *",
	    " * * * * * * \n * * * * * * ",
	    "\n"};

	for (int i = 0; i < 4; i++)
	{
		clearScreen();
		cout << COLOR_YELLOW << fireworks[i] << COLOR_RESET << endl;
		cout << " CONGRATULATIONS! " << endl;
		cout << COLOR_YELLOW << fireworks[(i + 2) % 4] << COLOR_RESET << endl;
#ifdef _WIN32
		Sleep(300);
#else
		usleep(300000);
#endif
	}
}

// Function to check if cell is valid
bool isValid(int row, int col)
{
	return (row >= 0) && (row < SIDE) && (col >= 0) && (col < SIDE);
}

// Check if cell contains mine
bool isMine(int row, int col, char board[][MAXSIDE])
{
	return (board[row][col] == '*');
}

// Get user move (row, col) and action (reveal or flag)
void makeMove(int *x, int *y, char *action)
{
	while (true)
	{
		printf("Enter your move (row, column) and action (r for reveal, f for flag) -> ");
		if (scanf("%d %d %c", x, y, action) != 3)
		{
			printf("Invalid input. Try again.\n");
			while (getchar() != '\n')
				; // Clear input buffer
			continue;
		}
		*action = tolower(*action);
		if (isValid(*x, *y))
		{
			if (*action == 'r' || *action == 'f')
			{
				break;
			}
		}
		printf("Invalid input. Try again.\n");
	}
	// Clear any remaining input after successful read
	while (getchar() != '\n')
		;
}

// Print the current game board with colors and animations
void printBoard(char myBoard[][MAXSIDE])
{
	clearScreen();

	// Print header with remaining mines
	cout << COLOR_CYAN << "MINESWEEPER" << COLOR_RESET << " - "
	     << COLOR_RED << "Mines left: " << remainingMines << COLOR_RESET << "\n\n";

	// Print column numbers
	cout << "    ";
	for (int i = 0; i < SIDE; i++)
	{
		printf("%2d ", i);
	}
	cout << "\n\n";

	// Print each row with row numbers
	for (int i = 0; i < SIDE; i++)
	{
		printf("%2d  ", i);
		for (int j = 0; j < SIDE; j++)
		{
			if (myBoard[i][j] == 'F')
			{
				cout << COLOR_RED << " F " << COLOR_RESET; // Flag
			}
			else if (myBoard[i][j] == '-')
			{
				cout << " . "; // Hidden cell
			}
			else if (myBoard[i][j] == '*')
			{
				cout << COLOR_RED << " * " << COLOR_RESET; // Mine
			}
			else if (myBoard[i][j] == '0')
			{
				cout << "   "; // Empty space
			}
			else
			{
				// Different colors for different numbers
				switch (myBoard[i][j])
				{
				case '1':
					cout << COLOR_BLUE;
					break;
				case '2':
					cout << COLOR_GREEN;
					break;
				case '3':
					cout << COLOR_RED;
					break;
				case '4':
					cout << COLOR_MAGENTA;
					break;
				default:
					cout << COLOR_YELLOW;
					break;
				}
				printf(" %c ", myBoard[i][j]);
				cout << COLOR_RESET;
			}
		}
		cout << "\n";
	}
	cout << "\n";
}

// Count adjacent mines for a cell
int countAdjacentMines(int row, int col, char realBoard[][MAXSIDE])
{
	int count = 0;

	// Check all 8 adjacent cells
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue; // Skip the current cell
			if (isValid(row + i, col + j))
			{
				if (isMine(row + i, col + j, realBoard))
				{
					count++;
				}
			}
		}
	}

	return count;
}

// Reveal cells recursively with animation
void revealCell(int row, int col, char myBoard[][MAXSIDE], char realBoard[][MAXSIDE],
		bool visited[][MAXSIDE], int *movesLeft)
{
	if (!isValid(row, col))
		return;
	if (visited[row][col])
		return;
	if (myBoard[row][col] != '-' && myBoard[row][col] != 'F')
		return;

	visited[row][col] = true;
	(*movesLeft)--;

	// Animation effect for revealing
	myBoard[row][col] = '?';
	printBoard(myBoard);
#ifdef _WIN32
	Sleep(50);
#else
	usleep(50000);
#endif

	int count = countAdjacentMines(row, col, realBoard);
	myBoard[row][col] = count + '0';

	if (count == 0)
	{
		// Reveal all adjacent cells if no adjacent mines
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0)
					continue;
				revealCell(row + i, col + j, myBoard, realBoard, visited, movesLeft);
			}
		}
	}
}

// Special first move reveal with animation
void firstMoveReveal(int row, int col, char myBoard[][MAXSIDE],
		     char realBoard[][MAXSIDE], int *movesLeft)
{
	bool visited[MAXSIDE][MAXSIDE] = {{false}};

	// First reveal the clicked cell with animation
	int count = countAdjacentMines(row, col, realBoard);
	myBoard[row][col] = '?';
	printBoard(myBoard);
#ifdef _WIN32
	Sleep(100);
#else
	usleep(100000);
#endif

	myBoard[row][col] = count + '0';
	(*movesLeft)--;
	visited[row][col] = true;

	// If it's a zero, reveal adjacent cells with animation
	if (count == 0)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0)
					continue;
				revealCell(row + i, col + j, myBoard, realBoard, visited, movesLeft);
			}
		}
	}

	printBoard(myBoard);
}

// Place mines randomly on the board with animation
void placeMines(int mines[][2], char realBoard[][MAXSIDE], int firstRow, int firstCol)
{
	bool mark[MAXSIDE * MAXSIDE] = {false};

	// Mark first click position and adjacent cells as unavailable for mines
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (isValid(firstRow + i, firstCol + j))
			{
				int pos = (firstRow + i) * SIDE + (firstCol + j);
				mark[pos] = true;
			}
		}
	}

	cout << "Placing mines";
	for (int i = 0; i < MINES;)
	{
		int random = rand() % (SIDE * SIDE);
		int x = random / SIDE;
		int y = random % SIDE;

		if (!mark[random])
		{
			mines[i][0] = x;
			mines[i][1] = y;
			realBoard[x][y] = '*';
			mark[random] = true;
			i++;

			// Simple animation effect
			if (i % 5 == 0)
			{
				cout << ".";
				cout.flush();
#ifdef _WIN32
				Sleep(50);
#else
				usleep(50000);
#endif
			}
		}
	}
	cout << "\n";
}

// Initialize the game boards
void initialise(char realBoard[][MAXSIDE], char myBoard[][MAXSIDE])
{
	loadingAnimation(200);

	cout << "Initializing game board";
	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
		{
			myBoard[i][j] = '-';
			realBoard[i][j] = '-';

			// Simple animation effect
			if ((i * SIDE + j) % 10 == 0)
			{
				cout << ".";
				cout.flush();
#ifdef _WIN32
				Sleep(10);
#else
				usleep(10000);
#endif
			}
		}
	}
	cout << "\n";
}

// Display game instructions with animation
void displayInstructions()
{
	clearScreen();
	cout << COLOR_CYAN << "=== MINESWEEPER INSTRUCTIONS ===" << COLOR_RESET << endl;

	const char *instructions[] = {
	    "1. The board is a grid of hidden cells, some containing mines.",
	    "2. Enter row and column numbers to reveal a cell (0-based indexing).",
	    "3. If you reveal a mine, you lose the game.",
	    "4. Numbers show how many mines are adjacent to that cell.",
	    "5. Use 'f' action to flag cells you think contain mines.",
	    "6. Reveal all non-mine cells to win the game.",
	    "7. First move is always safe and reveals a larger area."};

	for (size_t i = 0; i < 7; i++)
	{
		cout << instructions[i] << endl;
#ifdef _WIN32
		Sleep(200);
#else
		usleep(200000);
#endif
	}

	cout << "\nPress Enter to continue...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}

// Play the Minesweeper game with enhanced visuals
void playMinesweeper()
{
	char realBoard[MAXSIDE][MAXSIDE]; // Actual board with mines
	char myBoard[MAXSIDE][MAXSIDE];	  // Player's visible board
	int mines[MAXMINES][2];		  // Stores mine coordinates

	initialise(realBoard, myBoard);
	remainingMines = MINES;

	int movesLeft = SIDE * SIDE - MINES;
	bool gameOver = false;
	bool firstMove = true;
	int firstRow = 0, firstCol = 0;

	displayInstructions();

	while (!gameOver)
	{
		printBoard(myBoard);

		int x = 0, y = 0;
		char action = '\0';
		makeMove(&x, &y, &action);

		if (firstMove)
		{
			// First move - place mines after first click to ensure it's safe
			firstRow = x;
			firstCol = y;
			placeMines(mines, realBoard, firstRow, firstCol);
			firstMove = false;

			// Reveal area around first click with animation
			firstMoveReveal(x, y, myBoard, realBoard, &movesLeft);
			continue;
		}

		if (action == 'f')
		{
			// Flag or unflag the cell
			if (myBoard[x][y] == 'F')
			{
				myBoard[x][y] = '-';
				remainingMines++;
			}
			else if (myBoard[x][y] == '-')
			{
				myBoard[x][y] = 'F';
				remainingMines--;
			}
		}
		else if (action == 'r')
		{
			// Reveal the cell
			if (myBoard[x][y] == 'F')
			{
				cout << COLOR_YELLOW << "Cell is flagged. Unflag it first." << COLOR_RESET << endl;
#ifdef _WIN32
				Sleep(1000);
#else
				usleep(1000000);
#endif
				continue;
			}

			if (isMine(x, y, realBoard))
			{
				// Game over - reveal all mines with explosion animation
				for (int i = 0; i < MINES; i++)
				{
					myBoard[mines[i][0]][mines[i][1]] = '*';
				}
				printBoard(myBoard);
				explosionAnimation();
				cout << COLOR_RED << "\nYou stepped on a mine! Game Over!\n"
				     << COLOR_RESET;
				gameOver = true;
			}
			else
			{
				// Reveal the cell with animation
				bool visited[MAXSIDE][MAXSIDE] = {{false}};
				revealCell(x, y, myBoard, realBoard, visited, &movesLeft);

				if (movesLeft == 0)
				{
					celebrationAnimation();
					printBoard(myBoard);
					cout << COLOR_GREEN << "\nCongratulations! You cleared the minefield!\n"
					     << COLOR_RESET;
					gameOver = true;
				}
			}
		}
	}
}

// Choose difficulty level with visual effects
void chooseDifficultyLevel()
{
	clearScreen();
	cout << COLOR_CYAN << "=== CHOOSE DIFFICULTY LEVEL ===" << COLOR_RESET << endl;
	cout << "0 - Beginner (9x9, 10 mines)\n";
	cout << "1 - Intermediate (16x16, 40 mines)\n";
	cout << "2 - Advanced (24x24, 99 mines)\n";

	int level;
	while (true)
	{
		cout << "Enter your choice (0-2): ";
		if (!(cin >> level))
		{
			cout << COLOR_RED << "Invalid input. Please enter a number." << COLOR_RESET << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		if (level >= BEGINNER && level <= ADVANCED)
		{
			break;
		}
		cout << COLOR_RED << "Invalid choice. Please try again." << COLOR_RESET << endl;
	}

	switch (level)
	{
	case BEGINNER:
		SIDE = 9;
		MINES = 10;
		break;
	case INTERMEDIATE:
		SIDE = 16;
		MINES = 40;
		break;
	case ADVANCED:
		SIDE = 24;
		MINES = 99;
		break;
	}
	remainingMines = MINES;

	// Confirmation animation
	cout << COLOR_GREEN << "\nDifficulty set to ";
	switch (level)
	{
	case BEGINNER:
		cout << "BEGINNER";
		break;
	case INTERMEDIATE:
		cout << "INTERMEDIATE";
		break;
	case ADVANCED:
		cout << "ADVANCED";
		break;
	}
	cout << " (" << SIDE << "x" << SIDE << " grid, " << MINES << " mines)" << COLOR_RESET << endl;

#ifdef _WIN32
	Sleep(1500);
#else
	usleep(1500000);
#endif
}

int main()
{
	srand(static_cast<unsigned int>(time(NULL))); // Seed random number generator

	bool playAgain = true;
	while (playAgain)
	{
		// Title animation
		clearScreen();
		cout << COLOR_CYAN;
		cout << "  __  __ _  ___  ___ _____ _____ _____________ _____" << endl;
		cout << " |  \\/  | |/ _ \\/ _ \\_   | ____|__ /___ /___ /" << endl;
		cout << " | |\\/| | | | | | | | | | |  |   | \\ |_ \\ |_ \\" << endl;
		cout << " | |  | | | || | || | | | |___ __) |) |_) |" << endl;
		cout << " ||  |||\\/\\/ || ||//_/" << COLOR_RESET << endl;

#ifdef _WIN32
		Sleep(1000);
#else
		usleep(1000000);
#endif

		chooseDifficultyLevel();
		playMinesweeper();

		char response;
		cout << "\nWould you like to play again? (y/n): ";
		cin >> response;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		playAgain = (tolower(response) == 'y');
	}

	cout << COLOR_CYAN << "\nThanks for playing Minesweeper!\n"
	     << COLOR_RESET;
	return 0;
}