#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include "Random.h"

class Lattice
{
public:
	Lattice(Random* Rnd, int Deg);
	virtual ~Lattice();
	inline int get_Deg() const { return Deg_; }
	inline int get_N() const { return N_; }
protected:
	Random* Rnd_;
	int Deg_; // Degree of triangular lattice
	int N_; // Max of i, j of S(i, j)
	int Nc_; // Max index of cube lattice
};

