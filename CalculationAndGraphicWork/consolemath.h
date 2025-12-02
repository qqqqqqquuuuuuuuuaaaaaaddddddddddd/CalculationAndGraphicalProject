#pragma once

enum EquationSolvingMethods
{
	Bisection,
	Chord
};

enum IntegralSolvingMethods
{
	Rectangle,
	Trapezoid
};

float GetMin(float a, float b);
float GetMax(float a, float b);
float SolveEquation(EquationSolvingMethods solvingMethod);
float SolveIntegral(IntegralSolvingMethods solvingMethod);