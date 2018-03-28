/*
 * SpinLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#include "SpinLattice.h"
using namespace std;

#define X 0
#define Y 1
#define Z 2

#define Q 0
#define R 1


SpinLattice::SpinLattice(int Deg)
	: Lattice(Deg), S_(2 * Deg + 1, vector<double*>(2 * Deg + 1, NULL))
{
	double rnd = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			S_[i][j] = new double;
			if ((i + j > Deg_ - 1) && (i + j < Nc_ + N_ - 1)) {
				rnd = dist(mt);
				if (rnd < 0.5) {
					*S_[i][j] = 1;
				}
				else {
					*S_[i][j] = -1;
				}
			}
			else {
				*S_[i][j] = 0;
			}
		}
	}

	// Set Boundary references

	for (int i = 0; i < Nc_; ++i) {
		S_[i][N_ - 1] = S_[i + Deg_][0];
	}

	for (int j = 1; j < Nc_; ++j) {
		S_[N_ - 1][j] = S_[0][Deg_ + j];
	}

	for (int i = 0; i < Nc_; i ++) {
		S_[Nc_ - 1 + i][N_ - 1 - i] = S_[i][Deg_ - i];
	}
}

SpinLattice::~SpinLattice()
{
	// TODO Auto-generated constructor stub
}

void SpinLattice::Printout(string suppl) const
{
	string path = "./Outputfiles/Spin" + suppl + ".dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	double Spin = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			Spin = SpinLattice::get_Spin_ax(i, j);
			*outputFileSpin << i << "\t" << j << "\t" << Spin << endl;
		}
	}
	outputFileSpin->close();
	delete outputFileSpin;
}

// Helper functions
vector<int> SpinLattice::Axial_to_cube(int q, int r) const
{
	vector<int> cube(3);
    cube[X] = q - Nc_;
    cube[Z] =  - (r - Nc_);
    cube[Y] = -cube[X] - cube[Z];
    return cube;
}

vector<int> SpinLattice::Cube_to_axial(int x, int y, int z) const
{
	vector<int> ax(2);
	ax[Q] = x + Nc_;
	ax[R] =  - z + Nc_;
    return ax;
}

int SpinLattice::make_cube_periodic(int x) const
{
	return ((x + Nc_) % (N_ - 1)) - Nc_;
}

// Takes two cubic coordinates x = (x1, x2, x3) and y = (y1, y2, y3) and verifies if they are at the same point. does not work...
bool SpinLattice::Cube_same_position(int x1, int x2, int x3, int y1, int y2, int y3)
{
	bool flag = true;
	flag = flag && (SpinLattice::make_cube_periodic(x1) == SpinLattice::make_cube_periodic(y1));
	flag = flag && (SpinLattice::make_cube_periodic(x2) == SpinLattice::make_cube_periodic(y2));
	flag = flag && (SpinLattice::make_cube_periodic(x3) == SpinLattice::make_cube_periodic(y3));
	return flag;
}

// Getters and Setters

// Without periodic boundary !!
double SpinLattice::get_Spin_ax(int q, int r) const
{
	return *S_[q][r];
}

// With periodic boundary !! not working
double SpinLattice::get_Spin_cube_periodic(int x, int y, int z) const
{
	x = SpinLattice::make_cube_periodic(x);
	y = SpinLattice::make_cube_periodic(y);
	z = SpinLattice::make_cube_periodic(z);

	vector<int> ax = SpinLattice::Cube_to_axial(x, y, z);
	return *S_[ax[Q]][ax[R]];
}

// Without periodic Boundary!!
void SpinLattice::set_Spin_ax(int q, int r, double val) {
	*S_[q][r] = val;
}

// With periodic Boundary!! not working
void SpinLattice::set_Spin_cube(int x, int y, int z, double val) {
	x = SpinLattice::make_cube_periodic(x);
	y = SpinLattice::make_cube_periodic(y);
	z = SpinLattice::make_cube_periodic(z);

	vector<int> ax = SpinLattice::Cube_to_axial(x, y, z);
	*S_[ax[Q]][ax[R]] = val;
}
