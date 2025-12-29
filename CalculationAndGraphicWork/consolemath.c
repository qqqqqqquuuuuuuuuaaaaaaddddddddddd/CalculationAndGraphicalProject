#include <math.h>
#include <limits.h>
#include <stdio.h>

#include "consolemath.h"

void MakeCalculationsForTable(const int n, float a, float b, float x[16], float func1[16], float func2[16])
{
	float dx = fabsf(b - a) / (n - 1);
	x[0] = a;

	for (int i = 0; i < n; i++)
	{
		func1[i] = FUNC1(x[i]);
		func2[i] = FUNC2(x[i]);
		if (i < n - 1) x[i + 1] = x[i] + dx;
	}
}

float GetMin(float arr[16])
{
	float minValue = arr[0];
	for (int i = 0; i < 15; i++)
	{
		if (minValue > arr[i + 1]) minValue = arr[i + 1];
	}

	return minValue;
}

float GetMax(float arr[16])
{
	float maxValue = arr[0];
	for (int i = 0; i < 15; i++)
	{
		if (maxValue < arr[i + 1]) maxValue = arr[i + 1];
	}

	return maxValue;
}

float SolveEquation(EquationSolvingMethods solvingMethod, float a, float b)
{
	switch (solvingMethod)
	{
	case BISECTION:
		return SolveEquationUsingBisection(a, b);
	case CHORD:
		return SolveEquationUsingChord(a, b);
	}
}

float SolveIntegral(IntegralSolvingMethods solvingMethod, float a, float b)
{
	switch (solvingMethod)
	{
	case RECTANGLE:
		return SolveIntegralUsingRectangle(a, b);
	case TRAPEZOID:
		return SolveIntegralUsingTrapezoid(a, b);
	}
}

static float SolveEquationUsingBisection(float a, float b)
{
	float epsilon = 0.0001f;
	float funcA = EQUATION_FUNC(a);
	float funcB = EQUATION_FUNC(b);
	float funcMid = NAN;
	float dx = NAN;
	int iteration = 0;

	if (fabsf(funcA) < epsilon) return a;
	if (fabsf(funcB) < epsilon) return b;
	if (funcA * funcB >= 0) return NAN;

	do
	{
		dx = (a + b) / 2.0f;
		funcMid = EQUATION_FUNC(dx);
		if (fabsf(funcMid) <= epsilon) return dx;
		if (funcA * funcMid < 0)
		{
			b = dx;
			funcB = funcMid;
		}
		else
		{
			a = dx;
			funcA = funcMid;
		}

		iteration++;
	} while (fabsf(b - a) >= epsilon);

	return (a + b) / 2.0f;
}


static float SolveEquationUsingChord(float a, float b)
{
	float funcA = EQUATION_FUNC(a);
	float funcB = EQUATION_FUNC(b);
	float bNext = 0.f;
	float epsilon = 0.001f;
	int iteration = 0;
	
	do
	{
		bNext = b - (a - b) / (funcA - funcB) * funcB;
		b = bNext;
		funcB = EQUATION_FUNC(b);
	} while (fabsf(funcB) >= epsilon);

	return b;
}

static float SolveIntegralUsingRectangle(float a, float b)
{
	const int n = 18;
	const float step = (b - a) / n;
	float s = 0.f;
	for (int i = 0; i < n; i++)
	{
		s += step * INTEGRAL_FUNC(a);
		a += step;
	}

	return s;
}

static float SolveIntegralUsingTrapezoid(float a, float b)
{
	const int n = 18;
	const float step = (b - a) / n;
	float s = (INTEGRAL_FUNC(a) + INTEGRAL_FUNC(b)) * 0.5f;
	for (int i = 1; i < n - 1; i++)
	{
		a += step;
		s += INTEGRAL_FUNC(a);
	}

	s *= step;
	return s;
}

