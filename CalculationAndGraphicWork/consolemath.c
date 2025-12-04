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
		return SolveEquationUsingChord();
	}
}

float SolveIntegral(IntegralSolvingMethods solvingMethod)
{
	return 0.0f;
}

#define EQUATION_FUNC(X) (powf((X), 3) + 3 * (X) + 2)
static float SolveEquationUsingBisection(float a, float b)
{
	float step = 0.001f;
	float x1 = EQUATION_FUNC(a);
	float x2 = EQUATION_FUNC(b);
	int iteration = 0;

	while (fabs(b - a) > step)
	{
		float xi = (a + b) / 2;
		float x = EQUATION_FUNC(xi);

		if ((x < 0 && x1 < 0) || (x > 0 && x1 > 0))
		{
			a = xi;
			x1 = EQUATION_FUNC(a);
		}
		else
		{
			b = xi;
			x2 = EQUATION_FUNC(b);
		}

		printf("Итерация %i: %f\n", iteration, xi);
		iteration++;
	}

	return (a + b) / 2;
}

static float SolveEquationUsingChord()
{

}