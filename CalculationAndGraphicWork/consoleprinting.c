#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>

#include "consoleprinting.h"
#include "consolemath.h"

#define FOREGROUND_DEFAULT 7

#define ANSI_RED "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

#define UFO_FLAGS_GO_DOWN 1
#define UFO_FLAGS_GO_RIGHT 1 << 1
#define UFO_FLAGS_FREEZE_VERTICAL 1 << 2
#define UFO_FLAGS_FREEZE_HORIZONTAL 1 << 3

float x[16], func1[16], func2[16], dx, a = 0, b = 3.14159f, n = 16;

static char outputStrings[6][64] = {
	{"[x] Нахождение min/max\n"},
	{"[x] Нарисовать график функции\n"},
	{"[ ] Вычисление уравнения\n"},
	{"[ ] Вычисление интеграла\n"},
	{"[ ] Авторы\n"},
	{"[ ] Выход\n"}
};

void PrepareMathOperations()
{
	MakeCalculationsForTable(n, a, b, x, func1, func2);
}

#define DIVIDER "+----------------------------------------------------------------+\n"
void DrawFunctionsValuesTable(HANDLE consoleHandle)
{
	printf(DIVIDER);
	printf("|\tx\t|\tF1(x)\t\t |\tF2(x)\t\t |\n");
	printf(DIVIDER);

	float minFunc1 = GetMin(func1);
	float maxFunc1 = GetMax(func1);
	float minFunc2 = GetMin(func2);
	float maxFunc2 = GetMax(func2);

	for (int i = 0; i < n; i++)
	{
		if (func1[i] == minFunc1 || func1[i] == maxFunc1)
		{
			printf("|\t%i\t|\t" ANSI_RED "%f" ANSI_RESET "\t | \t % f\t | \n", i, func1[i], func2[i]);
			printf(DIVIDER);
			continue;
		}

		if (func2[i] == minFunc2 || func2[i] == maxFunc2)
		{
			printf("|\t%i\t|\t%f\t |\t" ANSI_RED "%f" ANSI_RESET "\t |\n", i, func1[i], func2[i]);
			printf(DIVIDER);
			continue;
		}
		printf("|\t%i\t|\t%f\t |\t%f\t |\n", i, func1[i], func2[i]);
		printf(DIVIDER);
	}

	printf("\n\n");
	printf("Минимальное значение F1(x): %f\n", minFunc1);
	printf("Максимальное значение F1(x): %f\n", maxFunc1);
	printf("Минимальное значение F2(x): %f\n", minFunc2);
	printf("Максимальное значение F2(x): %f", maxFunc2);
}

void DrawEquationValuesTable(HANDLE consoleHandle, EquationSolvingMethods solvingMethod)
{
	printf("Результат бисекции: %f", SolveEquation(BISECTION, a, b));
}
#undef DIVIDER

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

	for (int i = 0; i < 3; i++) printf(outputStrings[i]);
}

void Select(int choice, HANDLE consoleHandle)
{
	ClearConsole();
	switch (choice)
	{
	case 0:
		DrawFunctionsValuesTable(consoleHandle);
		break;
	case 1:
		// TODO: Choice of functions
		DrawCharts(consoleHandle, func1);
		break;
	case 2:
		DrawEquationValuesTable(consoleHandle, BISECTION);
		break;
	case 3:
		break;
	case 4:
		PrintAuthors(consoleHandle);
		break;
	case 5:
		system("exit");
	}
}

void PrintChoiceMessages(int choice, int previousChoice)
{
	ClearConsole();

	for (int i = 0; i < 6; i++)
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

void DrawCharts(HANDLE consoleHandle, const float arr[16])
{
	HWND hwnd = GetConsoleWindow();
	RECT rect;
	GetClientRect(hwnd, &rect);
	HDC hdc = GetDC(hwnd);
	HPEN pen = CreatePen(PS_DASHDOT, 2, RGB(255, 100, 100));
	HBRUSH brush = CreateSolidBrush(RGB(0, 100, 177));
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);

	int centerX = rect.right / 2;
	int centerY = rect.bottom / 2;
	int k = 100;
	float step = 0.001f;

	for (int i = a; i <= b; i += step)
	{
		float funcValue = FUNC1(i);
		if (i == a)
		{
			SelectObject(hdc, pen);
			MoveToEx(hdc, centerX + k * i, centerY - k * funcValue, NULL);
		}
		else LineTo(hdc, centerX + k * i, centerY - k * i);
	}
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