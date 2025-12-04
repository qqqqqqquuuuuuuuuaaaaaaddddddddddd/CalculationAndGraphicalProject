#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <windows.h>

#include "consoleprinting.h"

#define KEY_ESCAPE 27
#define KEY_ARROW_UP 72
#define KEY_ARROW_DOWN 80
#define KEY_ENTER 13

static BOOL isInMainMenu = FALSE;

int main(void)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	HideConsoleCursor(consoleHandle);

	setlocale(LC_ALL, "Ru");

	ShowSplashScreen(consoleHandle);
	ClearConsole();

	PrepareMathOperations();
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

		if (choice < 0) choice = 5;
		else if (choice > 5) choice = 0;
		PrintChoiceMessages(choice, previousChoice);
	}

	return 0;
}
