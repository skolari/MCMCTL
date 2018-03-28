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
			Spin = this->get_Spin(i, j);
			*outputFileSpin << i << "\t" << j << "\t" << Spin << endl;
		}
	}
	outputFileSpin->close();
	delete outputFileSpin;
}

// Helper functions
vector<int> SpinLattice::Axial_to_cube(int q, int r) const
{
	vector<int> cube(3, 0);
    cube[X] = -(q - Deg_);
    cube[Z] =  - (r - Deg_);
    cube[Y] = -cube[X] - cube[Z];
    return cube;
}

vector<int> SpinLattice::Cube_to_axial(int x, int y, int z) const
{
	vector<int> ax(2);
	ax[Q] = - x + Deg_;
	ax[R] =  - z + Deg_;
    return ax;
}

// Takes two coordinates x = (x1, x2) and y = (y1, y2) and verifies if they are at the same point.
bool SpinLattice::Same_point(int i1, int j1, int i2, int j2) const
{
	return (i1 == i2) && (j1 == j2);
}

// Getters and Setters


void SpinLattice::set_Spin(int i, int j, double val) {
	*S_[i][j] = val;
}

void SpinLattice::set_Spin_cube(int x, int y, int z, double val) {
	vector<int> ax = this->Cube_to_axial(x, y, z);
	*S_[ax[Q]][ax[R]] = val;
}

vector<int> SpinLattice::fix_bc(int i, int j){
	if (j == N_ - 1  && i < Nc_) {
		i = i + Deg_;
		j = 0;
	}
	if(i == N_ - 1 && j > 0 && j < Nc_) {
		i = 0;
		j = j + Deg_;
	}
	if(j + i == (N_ - 1 + Deg_) && j < N_ - 1 && i < N_ - 1){
		i = i - Deg_;
		j = j - Deg_;
	}
	if (j == -1) {
		i = i - Deg_;
		j = N_ - 2;
	}
	if (i == -1) {
		i = N_ - 2;
		j = j - Deg_;
	}
	if (i + j == Deg_ - 1) {
		i = i + Deg_;
		j = j + Deg_;
	}
	vector<int> coord = {i, j};
	return coord;
}

vector<int> SpinLattice::step_dir(int i, int j, int d) {
	assert(abs(d) == 1 || abs(d) == 2 || abs(d) == 3);
	vector<int> step(2);
	if (d == 1 || d == -1) {
		i = i + d;
		step = this->fix_bc(i, j);
	} else if (d == 2 || d == -2){
		d = d/2;
		j = j + d;
		step = this->fix_bc(i, j);
	} else if (d == 3 || d == -3){
		d = d/2;
		j = j + d;
		i = i - d;
		step = this->fix_bc(i, j);
	}
	return step;
}
