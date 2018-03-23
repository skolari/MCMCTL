#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>

using namespace std;

class Lattice
{
public:
	Lattice(int Deg);
	~Lattice();

	// vector< vector<double> > fromSpinToDimerAdj(vector< vector<double> > S);
	// vector< vector<double> > fromDimerToSpin(vector< vector<double> > D);
	void printoutSpin(string suppl); // Creates outputfile of the Spin Lattice
	void printoutDimerAdj(string suppl);

protected:
	// Attributes
	vector< vector<double> > S_;				// Spin lattice S(i, j)
	vector< vector<double> > Dadj_;				// Dimer Adj. Matrix
	int Deg_; // Degree of triangular lattice
	int N_; // Max of i, j of S(i, j)

	//const double J1_; // J1
	//const double J2_; // J2
	//const double J3_; // J3
	

	// Methods
	bool ifUpperBoundary(int i, int j);
	//vector< vector<double> > fromSpinToDimer(vector< vector<double> > S);
	//vector< vector<double> > fromDimerToSpin(vector< vector<double> > D);

private:
	// random
	random_device rd;							// Random device // @suppress("Type cannot be resolved")
	mt19937 mt;									// mt19937 // @suppress("Type cannot be resolved")
	uniform_real_distribution<double> dist;		// Uniform real distribution // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")
};

