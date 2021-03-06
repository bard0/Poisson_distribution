#pragma once

#include "pch.h"
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;
static double stairWidth[257], stairHeight[256];
const double x1 = 7.69711747013104972;
const double A = 3.9496598225815571993e-3; /// area under rectangle
unsigned long long BasicRandGenerator() {
	unsigned long long randomVariable;
	// some magic here
	randomVariable = std::rand();
	return randomVariable;
}
double Uniform(double a, double b) {
	return a + BasicRandGenerator() * (b - a) / RAND_MAX;
}
void setupExpTables() {
	// coordinates of the implicit rectangle in base layer
	stairHeight[0] = exp(-x1);
	stairWidth[0] = A / stairHeight[0];
	// implicit value for the top layer
	stairWidth[256] = 0;
	for (unsigned i = 1; i <= 255; ++i)
	{
		// such x_i that f(x_i) = y_{i-1}
		stairWidth[i] = -log(stairHeight[i - 1]);
		stairHeight[i] = stairHeight[i - 1] + A / stairWidth[i];
	}
}

double ExpZiggurat() {
	int iter = 0;
	do {
		int stairId = BasicRandGenerator() & 255;
		double x = Uniform(0, stairWidth[stairId]); // get horizontal coordinate
		if (x < stairWidth[stairId + 1]) /// if we are under the upper stair - accept
			return x;
		if (stairId == 0) // if we catch the tail
			return x1 + ExpZiggurat();
		if (Uniform(stairHeight[stairId - 1], stairHeight[stairId]) < exp(-x)) // if we are under the curve - accept
			return x;
		// rejection - go back
	} while (++iter <= 1e9); // one billion should be enough to be sure there is a bug
	return NAN; // fail due to some error
}
double Exponential(double rate) {
	return ExpZiggurat() / rate;
}
double PoissonExponential(double rate) {
	int k = -1;
	double s = 0;
	do {
		s += Exponential(1);
		++k;
	} while (s < rate);
	return k;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	double a = 0;
	int n = 0;
	std::cout << "Введите плотность распределения" << endl;
	std::cin >> a;
	std::cout << "Введите число величин" << endl;
	std::cin >> n;
	ofstream file("file1.txt");
	if (!file)
		exit(1);
	for (int i = 0; i < n; i++) {
		std::cout << PoissonExponential(a)<< " ";
		file << PoissonExponential(a) << endl;
	}
	file.close();
	return 0;
}


