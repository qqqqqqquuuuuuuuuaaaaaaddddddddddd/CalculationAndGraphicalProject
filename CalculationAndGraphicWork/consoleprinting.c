#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>

#include "consoleprinting.h"
#include "consolemath.h"

#define FOREGROUND_DEFAULT 7

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_RESET "\x1b[0m"

#define UFO_FLAGS_GO_RIGHT 1 << 1

float x[16], func1[16], func2[16], dx, a = 0.f, b = 3.14159f; 
int n = 16;

static char outputStrings[6][64] = {
	{"[x] Нахождение min/max\n"},
	{"[ ] Нарисовать график функции\n"},
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
		if (func1[i] == minFunc1)
		{
			printf("|\t%i\t|\t" ANSI_GREEN "%f" ANSI_RESET "\t | \t %f\t | \n", i, func1[i], func2[i]);
			printf(DIVIDER);
			continue;
		}

		if (func1[i] == maxFunc1)
		{
			printf("|\t%i\t|\t" ANSI_RED "%f" ANSI_RESET "\t | \t %f\t | \n", i, func1[i], func2[i]);
			printf(DIVIDER);
			continue;
		}

		if (func2[i] == minFunc2)
		{
			printf("|\t%i\t|\t%f\t |\t" ANSI_GREEN "%f" ANSI_RESET "\t |\n", i, func1[i], func2[i]);
			printf(DIVIDER);
			continue;
		}

		if (func2[i] == maxFunc2)
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
	printf("Максимальное значение F2(x): %f\n", maxFunc2);
}

void DrawEquationValuesTable(HANDLE consoleHandle)
{
	int error;
	float x1, x2;
	do
	{
		error = 0;
		printf("a = ");
		error += scanf_s("%f", &x1);
		while (getchar() != '\n');
		printf("b = ");
		error += scanf_s("%f", &x2);
		if (error == 0)
		{
			printf("\n\n Ошибка чтения значений. Нажмите любую клавишу");
			_getch();
			ClearConsole();
			while (getchar() != '\n');
		}
	} while (error == 0);

	float root = SolveEquation(BISECTION, x1, x2);
	char buffer[128];
	if (isnan(root)) snprintf(buffer, 128, "Корней нет");
	else _gcvt_s(buffer, 128, root, 4);
	printf("Результат бисекции: %s\n", buffer);

	root = SolveEquation(CHORD, x1, x2);
	if (isnan(root)) snprintf(buffer, 128, "Корней нет");
	else _gcvt_s(buffer, 128, root, 4);
	printf("Результат метода хорд: %s\n", buffer);
}

void DrawIntegralValuesTable(HANDLE consoleHandle)
{
	int error;
	float x1, x2;
	do
	{
		error = 0;
		printf("a = ");
		error += scanf_s("%f", &x1);
		while (getchar() != '\n');
		printf("b = ");
		error += scanf_s("%f", &x2);
		if (error == 0)
		{
			printf("\n\n Ошибка чтения значений. Нажмите любую клавишу");
			_getch();
			ClearConsole();
			while (getchar() != '\n');
		}
	} while (error == 0);

	printf("Результат метода прямоугольников: %f\n", SolveIntegral(RECTANGLE, x1, x2));
	printf("Результат метода трапеций: %f\n", SolveIntegral(TRAPEZOID, x1, x2));
}
#undef DIVIDER

void ShowSplashScreen(HANDLE consoleHandle)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);

	int width = csbi.srWindow.Right;
	int height = csbi.srWindow.Bottom;
	const int grassHeight = height - 3;

	COORD ufoCoords = { 0, 6 };
	COORD ufoTargetCoords = { 0, 1 };
	const int ufoFlyHeight = 1;
	char ufoFlags = 0;
	ufoFlags = UFO_FLAGS_GO_RIGHT;
	HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	BOOL isUfoBlue = TRUE;

	while (_kbhit() == 0)
	{
		SetConsoleTextAttribute(stdHandle, FOREGROUND_DEFAULT);

		isUfoBlue = !isUfoBlue;
		SetConsoleTextAttribute(stdHandle, isUfoBlue ? FOREGROUND_BLUE : FOREGROUND_RED);
		COORD printCoords = ufoCoords;
		SetConsoleCursorPosition(consoleHandle, printCoords);
		printf("     ___\n");
		printCoords.Y++;
		SetConsoleCursorPosition(consoleHandle, printCoords);
		printf("  ___/ \\___\n");
		printCoords.Y++;
		SetConsoleCursorPosition(consoleHandle, printCoords);
		printf(" /  '---'  \\\n");
		printCoords.Y++;
		SetConsoleCursorPosition(consoleHandle, printCoords);
		printf("'--_______--'\n");
		SetConsoleTextAttribute(stdHandle, FOREGROUND_INTENSITY);
		printCoords.Y++;
		SetConsoleCursorPosition(consoleHandle, printCoords);
		printf("     /=\\\n");
		printCoords.Y++;
		SetConsoleCursorPosition(consoleHandle, printCoords);
		printf("    /===\\\n");
		printCoords.Y++;
		SetConsoleCursorPosition(consoleHandle, printCoords);
		printf("   /=====\\\n");

		if (ufoCoords.X == 0) ufoFlags |= UFO_FLAGS_GO_RIGHT;
		if (ufoCoords.X == width - 14) ufoFlags &= ~(UFO_FLAGS_GO_RIGHT);

		switch (ufoFlags & UFO_FLAGS_GO_RIGHT)
		{
			case UFO_FLAGS_GO_RIGHT:
				ufoCoords.X += 3;
				break;
			default:
				ufoCoords.X -= 3;
				break;
		}

		SetConsoleTextAttribute(stdHandle, BACKGROUND_INTENSITY);
		for (int i = height; i >= grassHeight; i--)
		{
			COORD newCoords;
			newCoords.X = 0;
			newCoords.Y = i;

			if (i == grassHeight)
			{
				SetConsoleTextAttribute(stdHandle, FOREGROUND_DEFAULT);

				newCoords.X = width / 2 - 10;
				SetConsoleCursorPosition(consoleHandle, newCoords);
				printf("|| w-- ||");
				newCoords.Y--;
				SetConsoleCursorPosition(consoleHandle, newCoords);
				printf("||      (__)");
				newCoords.Y--;
				SetConsoleCursorPosition(consoleHandle, newCoords);
				if (ufoCoords.X > newCoords.X - 16 && ufoCoords.X < newCoords.X + 16) printf("`\\------(oo)");
				else printf("`\\------(^^)");
				newCoords.Y--;
				SetConsoleCursorPosition(consoleHandle, newCoords);
				printf("        (__)");
				break;
			}

			SetConsoleCursorPosition(consoleHandle, newCoords);
			for (int j = 0; j < width; j++)
			{
				SetConsoleTextAttribute(stdHandle, FOREGROUND_GREEN);
				if (i == height)
				{
					SetConsoleTextAttribute(stdHandle, BACKGROUND_INTENSITY);
					printf(" ");
					if (j == width / 2 - 16)
					{
						printf("Нажмите любую клавишу");
						j += 21;
					}
					continue;
				}
				
				printf("#");
			}
		}

		Sleep(500);
		ClearConsole();
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
		DrawEquationValuesTable(consoleHandle);
		break;
	case 3:
		DrawIntegralValuesTable(consoleHandle);
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
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(handle, &csbi);

	for (int i = 0; i < 6; i++)
	{
		COORD menuCoords = { csbi.srWindow.Right / 2 - 10,  csbi.srWindow.Bottom / 2 - 6 + i };
		SetConsoleCursorPosition(handle, menuCoords);
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
	Sleep(75);
	HWND hwnd = GetConsoleWindow();
	RECT rect;
	GetClientRect(hwnd, &rect);
	
	HDC hdc = GetDC(hwnd);
	
	int width = rect.right;
	int height = rect.bottom;
	int widthCenter = rect.right / 2;
	int heightCenter = rect.bottom / 2;
	int scale = (width > height) ? height / 16 : width / 16;

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	SelectObject(hdc, pen);

	MoveToEx(hdc, 0, heightCenter, NULL);
	LineTo(hdc, width, heightCenter);
	MoveToEx(hdc, widthCenter, height, NULL);
	LineTo(hdc, widthCenter, 0);

	short buffer[16];
	int nSize;

	for (int i = 1; i < 16; i++)
	{
		int scaledX = i * scale + widthCenter;
		MoveToEx(hdc, scaledX, heightCenter - 10, NULL);
		LineTo(hdc, scaledX, heightCenter + 10);
		nSize = wsprintf(buffer, TEXT("%d"), i);
		TextOut(hdc, scaledX - 5, heightCenter + 15, buffer, nSize);

		int scaledY = -i * scale + heightCenter;
		MoveToEx(hdc, widthCenter - 10, scaledY, NULL);
		LineTo(hdc, widthCenter + 10, scaledY);
		nSize = wsprintf(buffer, TEXT("%d"), i);
		TextOut(hdc, widthCenter + 15, scaledY, buffer, nSize);
	}

	pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	SelectObject(hdc, pen);

	for (int i = 0; i < 15; i++)
	{
		MoveToEx(hdc, x[i] * scale + widthCenter, func1[i] * scale + heightCenter, NULL);
		LineTo(hdc, x[i + 1] * scale + widthCenter, func1[i + 1] * scale + heightCenter);
	}
	nSize = wsprintf(buffer, TEXT("f1(x)"));
	TextOut(hdc, x[7] * scale + widthCenter + 15, func1[7] * scale + heightCenter + 15, buffer, nSize);

	pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	SelectObject(hdc, pen);
	for (int i = 0; i < 15; i++)
	{
		MoveToEx(hdc, x[i] * scale + widthCenter, (int)(func2[i] * scale * -0.01f) + heightCenter, NULL);
		LineTo(hdc, x[i + 1] * scale + widthCenter, (int)(func2[i + 1] * scale * -0.01f) + heightCenter);
	}

	nSize = wsprintf(buffer, TEXT("f2(x) / 100"));
	TextOut(hdc, x[7] * scale + widthCenter + 15, func2[7] * scale * -0.01f + heightCenter + 15, buffer, nSize);
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