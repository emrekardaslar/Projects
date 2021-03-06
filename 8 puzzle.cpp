#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <string>

#define DIM 3

#define OFFSET_X 2
#define OFFSET_Y 3

#define FRAME_TOP_LEFT_CH (char)218
#define FRAME_TOP_CH (char)196
#define FRAME_TOP_RIGHT_CH (char)191
#define FRAME_LEFT_CH (char)179
#define FRAME_RIGHT_CH (char)179
#define FRAME_BOTTOM_LEFT_CH (char)192
#define FRAME_BOTTOM_CH (char)196
#define FRAME_BOTTOM_RIGHT_CH (char)217

using namespace std;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN, INVALID };
eDirection dir;

int x, y;
int moves = 0;
char goal[3][3] = { { '1','2','3' }, { '4', '5', '6' }, { '7', '8', ' ' } };
char box[3][3]  = { { '8',' ','6' }, { '5', '4', '7' }, { '2', '3', '1' } };
//char box[3][3] = { { '1','2','3' },{ '4', '5', '6' },{ ' ', '7', '8' } }; (Can be tested easily)

void gotoxy(int x = 0, int y = 0)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(h, c);
}

void GoToXY(int x = OFFSET_X, int y = OFFSET_Y)
{
	gotoxy(x + OFFSET_X, y + OFFSET_Y);
}

void DrawFrameTop()
{
	GoToXY(0, 0);
	cout << FRAME_TOP_LEFT_CH;

	for (int i = 0; i < DIM + 2; i++) {
		GoToXY(i + 1, 0);
		cout << FRAME_TOP_CH;
	}
	GoToXY(DIM + 3, 0);
	cout << FRAME_TOP_RIGHT_CH;
}

void DrawFrameBody()
{
	for (int i = 0; i < DIM + 2; i++) {
		GoToXY(0, i + 1);
		cout << FRAME_LEFT_CH;
		GoToXY(1, i + 1);
		for (int j = 0; j < DIM + 2; j++)
			cout << " ";
		GoToXY(DIM + 3, i + 1);
		cout << FRAME_RIGHT_CH;
	}
}

void DrawFrameBottom()
{
	GoToXY(0, DIM + 3);
	cout << FRAME_BOTTOM_LEFT_CH;
	for (int i = 0; i < 5; i++) {
		GoToXY(i + 1, DIM + 3);
		cout << FRAME_BOTTOM_CH;
	}
	GoToXY(DIM + 3, DIM + 3);
	cout << FRAME_BOTTOM_RIGHT_CH;
}

void DrawFrame()
{
	system("cls");
	gotoxy(OFFSET_X, 1);
	cout << "8-PUZZLE ver. 1";
	
	DrawFrameTop();
	DrawFrameBody();
	DrawFrameBottom();
	
}

void DrawNumberOfMoves()
{
	GoToXY(0, DIM + 5);
	cout << "Number of moves: " << moves << endl;
}

bool getEmpty(int &x, int& y)
{
	x = -1;
	y = -1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (box[i][j] == ' ') {
				x = i;
				y = j;
				return true;
			}
		}
	}

	return false;
}

void input() {
	switch (_getch()) {
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'x':
			dir = STOP;
			break;
		default:
			dir = INVALID;
		}
}

void makeMove() {
	int x, y;
	bool isValid = getEmpty(x, y);
	
	if (!isValid)
		return;

	switch (dir) {
		case DOWN:
			if (x == 0)
				return;
			box[x][y] = box[x - 1][y];
			box[x - 1][y] = ' ';
			break;

		case UP:
			if (x == 2)
				return;
			box[x][y] = box[x + 1][y];
			box[x + 1][y] = ' ';
			break;

		case RIGHT:
			if (y == 0)
				return;
			box[x][y] = box[x][y - 1];
			box[x][y - 1] = ' ';
			break;

		case LEFT:
			if (y == 2)
				return;
			box[x][y] = box[x][y + 1];
			box[x][y + 1] = ' ';
			break;

		default:
			/* Nothing to do */
			return;
	}
	moves++;
}

bool isSolved()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (box[i][j] != goal[i][j])
				return false;
		}
	}

	return true;
}

void Draw()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			GoToXY(2*j+1, 2*i+1);
			cout << box[i][j];
		}
	}
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}


int main() 
{
	bool solved = false;

	ShowConsoleCursor(false);

	DrawFrame();
	DrawNumberOfMoves();

	do {
		Draw();
		input();
		if (dir != STOP && dir != INVALID)
		{
			makeMove();
			solved = isSolved();
			DrawNumberOfMoves();
		}
	} while (!solved && dir != STOP);

	if (solved)
		Draw();

	GoToXY(0, DIM + 7);

	if (solved)
		cout << "Congratulations!!!" << endl;
	else
		cout << "Bye!" << endl;

	ShowConsoleCursor(true);

	GoToXY(0, DIM + 9);

	return 0;
}