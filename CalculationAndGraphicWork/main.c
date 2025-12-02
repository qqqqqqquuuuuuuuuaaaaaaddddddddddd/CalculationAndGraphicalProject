#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <windows.h>

#include "consoleprinting.h"
#include "consolemath.h"

#define KEY_ESCAPE 27
#define KEY_ARROW_UP 72
#define KEY_ARROW_DOWN 80
#define KEY_ENTER 13

#define FOREGROUND_DEFAULT 7

#define UFO_FLAGS_GO_DOWN 1
#define UFO_FLAGS_GO_RIGHT 1 << 1
#define UFO_FLAGS_FREEZE_VERTICAL 1 << 2
#define UFO_FLAGS_FREEZE_HORIZONTAL 1 << 3

static BOOL isInMainMenu = FALSE;

char outputStrings[4][64] = {
	{"[x] Нахождение min/max\n"},
	{"[ ] Вычисление уравнения\n"},
	{"[ ] Вычисление интеграла\n"},
	{"[ ] Авторы\n"}
};

int main(void)
{

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	HideConsoleCursor(consoleHandle);

	setlocale(LC_ALL, "Ru");

	ShowSplashScreen(consoleHandle);
	ClearConsole();

	for (int i = 0; i < 3; i++) printf(outputStrings[i]);
	int keyInput, previousChoice, choice = 0;

	while ((keyInput = _getch()) != KEY_ESCAPE)
	{
		previousChoice = choice;

		switch (keyInput)
		{
			case KEY_ARROW_UP:
				choice--;
				break;
			case KEY_ARROW_DOWN:
				choice++;
				break;
			case KEY_ENTER:
				Select(choice, consoleHandle);
				continue;
			default:
				continue;
		}

		if (choice < 0) choice = 3;
		else if (choice > 3) choice = 0;
		PrintChoiceMessages(choice, previousChoice);
	}

	return 0;
}

void ShowSplashScreen(HANDLE consoleHandle)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);

	int width = csbi.srWindow.Right;
	int height = csbi.srWindow.Bottom;
	const int grassHeight = height - 2;

	/*
	*   /\
	*  -__-
	* 
	*/

	for (int i = height; i > grassHeight; i--)
	{
		COORD newCoords;
		newCoords.X = 0;
		newCoords.Y = i;

		SetConsoleCursorPosition(consoleHandle, newCoords);
		for (int i = 0; i < width; i++)
		{
			printf("#");
		}
	}

	COORD ufoCoords = { 0, 1 };
	COORD ufoTargetCoords = { 0, 1 };
	char ufoFlags = 0;

	while (_kbhit() == 0)
	{
		if (ufoCoords.X == ufoTargetCoords.X && ufoCoords.Y == ufoCoords.Y)
		{
			ufoTargetCoords.X = rand() % (width - 16) + 8;
		}

		if (ufoCoords.X != ufoTargetCoords.X)
		{
			if (ufoCoords.Y != height * 0.5)
			{
				ufoFlags &= !UFO_FLAGS_GO_DOWN;
				ufoFlags |= UFO_FLAGS_FREEZE_HORIZONTAL;
			}
			else
			{
				ufoFlags |= UFO_FLAGS_FREEZE_VERTICAL;
			}
		}

		SetConsoleCursorPosition(consoleHandle, ufoCoords);
		printf("   /\\\n");
		printf("  -__-");

		if (ufoCoords.Y >= grassHeight) ufoFlags |= UFO_FLAGS_GO_DOWN;
		if (ufoCoords.Y <= 0) ufoFlags &= !UFO_FLAGS_GO_DOWN;
		if (ufoCoords.X <= 0) ufoFlags |= UFO_FLAGS_GO_RIGHT;
		if (ufoCoords.X >= width) ufoFlags &= !UFO_FLAGS_GO_RIGHT;

		switch (ufoFlags & (UFO_FLAGS_GO_DOWN | UFO_FLAGS_GO_RIGHT))
		{
		case UFO_FLAGS_GO_DOWN:
			ufoCoords.Y++;
			break;
		case UFO_FLAGS_GO_RIGHT:
			ufoCoords.X++;
			break;
		case UFO_FLAGS_GO_DOWN | UFO_FLAGS_GO_RIGHT:
			ufoCoords.X++;
			ufoCoords.Y++;
			break;
		default:
			if (ufoFlags & UFO_FLAGS_FREEZE_HORIZONTAL != 0)
			{
				ufoCoords.X--;
			}

			if (ufoFlags & UFO_FLAGS_FREEZE_VERTICAL != 0)
			{
				ufoCoords.Y--;
			}
		}
	}
}

void Select(int choice, HANDLE consoleHandle)
{
	switch (choice)
	{
	case 2:
		ClearConsole();
		PrintAuthors(consoleHandle);
	}
}

void PrintChoiceMessages(int choice, int previousChoice)
{
	ClearConsole();

	for (int i = 0; i < 4; i++)
	{
		if (i == 0) outputStrings[previousChoice][1] = ' ';
		if (i == choice)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			outputStrings[i][1] = 'x';
			printf(outputStrings[i]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_DEFAULT);

			continue;
		}

		printf(outputStrings[i]);
	}
}

void ClearConsole()
{
	system("cls");
}

void PrintAuthors(HANDLE consoleHandle)
{
	CenterText(consoleHandle, "| Разработано студентом группы ИВТ-252 Гедройц Вячеславом |");
}

void HideConsoleCursor(HANDLE consoleHandle)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void CenterText(HANDLE consoleHandle, const char text[])
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);

	int consoleWidth = csbi.srWindow.Right;
	int textLength = strlen(text);
	int centerX = (consoleWidth - textLength) / 2;

	COORD pos = { centerX, csbi.srWindow.Bottom / 2 - 1 };

	SetConsoleCursorPosition(consoleHandle, pos);
	printf("+=========================================================+");

	pos.Y += 1;
	SetConsoleCursorPosition(consoleHandle, pos);
	printf("%s", text);

	pos.Y += 1;
	SetConsoleCursorPosition(consoleHandle, pos);
	printf("+=========================================================+");
}

float GetMin(float a, float b)
{
	return a > b ?
}