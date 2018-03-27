#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <cmath>

using namespace std;

class Lattice
{
public:
	Lattice(int Deg);
	virtual ~Lattice();

	//void printoutDimerAdj(string suppl);

protected:
	// Attributes
	//vector< vector<double> > Dadj_;				// Dimer Adj. Matrix
	int Deg_; // Degree of triangular lattice
	int N_; // Max of i, j of S(i, j)
	int Nc_; // Max index of cube lattice
	// int NDadj_; // size of Dadj_ matrix

	// Methods

	//vector< vector<double> > fromSpinToDimer(vector< vector<double> > S);
	//vector< vector<double> > fromSpinToDimerAdj(vector< vector<double> > S);
	//vector< vector<double> > fromDimerToSpin(vector< vector<double> > D);

	// random
	random_device rd;							// Random device // @suppress("Type cannot be resolved")
	mt19937 mt;									// mt19937 // @suppress("Type cannot be resolved")
	uniform_real_distribution<double> dist;		// Uniform real distribution // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")

private:
	// Methods

	//int getDadjInd(int i, int j);
};

