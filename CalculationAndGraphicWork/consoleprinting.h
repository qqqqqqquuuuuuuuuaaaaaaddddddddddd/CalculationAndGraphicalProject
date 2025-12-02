#pragma once

void Select(int choice, HANDLE consoleHandle);
void HideConsoleCursor(HANDLE consoleHandle);
void CenterText(HANDLE consoleHandle, const char text[]);
void ClearConsole();
void ShowSplashScreen(HANDLE consoleHandle);

void PrintChoiceMessages(int choice, int previousChoice);
void PrintAuthors(HANDLE consoleHandle);
