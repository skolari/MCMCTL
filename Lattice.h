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
	inline int get_Deg() const { return Deg_; }
	inline int get_N() const { return N_; }
protected:
	// Attributes
	int Deg_; // Degree of triangular lattice
	int N_; // Max of i, j of S(i, j)
	int Nc_; // Max index of cube lattice

	// random
	std::random_device rd;							// Random device // @suppress("Type cannot be resolved")
	std::mt19937 mt;									// mt19937 // @suppress("Type cannot be resolved")
	std::uniform_real_distribution<double> dist;		// Uniform real distribution // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")

};

