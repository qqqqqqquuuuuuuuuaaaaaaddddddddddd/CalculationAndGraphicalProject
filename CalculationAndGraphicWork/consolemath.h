#pragma once

#include <math.h>

#define FUNC1(X) (2 * sinf(2 * (X)) + 1)
#define FUNC2(X) (powf(((X) + 5), 3) * (1 + powf(sinf((X)), 2)))

#define EQUATION_FUNC(X) (powf((X), 3) + 3 * (X) + 2)
#define INTEGRAL_FUNC(X) (1.f / ((X) + sqrtf(fabsf(cosf((X))))))

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
float SolveIntegral(IntegralSolvingMethods solvingMethod, float a, float b);
static float SolveEquationUsingBisection(float a, float b);
static float SolveEquationUsingChord(float a, float b);
static float SolveIntegralUsingRectangle(float a, float b);
static float SolveIntegralUsingTrapezoid(float a, float b);