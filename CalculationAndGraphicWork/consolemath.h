#pragma once

#define FUNC1(X) 2 * sinf(2 * X) + 1
#define FUNC2(X) powf((X + 5), 3) * (1 + powf(sinf(X), 2))

typedef enum
{
	BISECTION,
	CHORD
} EquationSolvingMethods;

typedef enum
{
	RECTANGLE,
	TRAPEZOID
} IntegralSolvingMethods;

void MakeCalculationsForTable(const int n, float a, float b, float x[16], float func1[16], float func2[16]);
float GetMin(float arr[16]);
float GetMax(float arr[16]);
float SolveEquation(EquationSolvingMethods solvingMethod, float a, float b);
static float SolveEquationUsingBisection(float a, float b);
float SolveIntegral(IntegralSolvingMethods solvingMethod);