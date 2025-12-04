#pragma once

#include "consolemath.h"

void Select(int choice, HANDLE consoleHandle);
void HideConsoleCursor(HANDLE consoleHandle);
void CenterText(HANDLE consoleHandle, const char text[]);
void ClearConsole();
void ShowSplashScreen(HANDLE consoleHandle);
void DrawFunctionsValuesTable(HANDLE consoleHandle);
void PrepareMathOperations();

void PrintChoiceMessages(int choice, int previousChoice);
void PrintAuthors(HANDLE consoleHandle);

void DrawCharts(HANDLE consoleHandle, float arr[16]);
void DrawEquationValuesTable(HANDLE consoleHandle, EquationSolvingMethods solvingMethod);