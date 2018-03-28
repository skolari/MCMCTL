#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <cmath>


class Lattice
{
public:
	Lattice(int Deg);
	virtual ~Lattice();


protected:
	// Attributes
	int Deg_; // Degree of triangular lattice
	int N_; // Max of i, j of S(i, j)
	int Nc_; // Max index of cube lattice

	// random
	random_device rd;							// Random device // @suppress("Type cannot be resolved")
	mt19937 mt;									// mt19937 // @suppress("Type cannot be resolved")
	uniform_real_distribution<double> dist;		// Uniform real distribution // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")

};

