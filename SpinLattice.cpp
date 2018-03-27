/*
 * SpinLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#include "SpinLattice.h"

#define X 0
#define Y 1
#define Z 2

#define Q 0
#define R 1


SpinLattice::SpinLattice(int Deg)
	: Lattice(Deg), S_(2 * Deg + 1, vector<double>(2 * Deg + 1, 0))
{
	double rnd = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (Lattice::ifUpperBoundary(i, j) != 0){
				S_[i][j] = 0;
			}
			else if ((i + j > Deg_ - 1) && (i + j < 2 * N_ - Deg_ - 1)) {
				rnd = dist(mt);
				if (rnd < 0.5) {
					S_[i][j] = 1;
				}
				else {
					S_[i][j] = -1;
				}
			}
			else {
				S_[i][j] = 0;
			}
		}
	}
}

SpinLattice::~SpinLattice()
{
	// TODO Auto-generated constructor stub
}

void SpinLattice::Printout(string suppl)
{
	string path = "./Outputfiles/Spin" + suppl + ".dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	int iUB = 0;
	double Spin = 0;
	vector<int> coord(2);
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			iUB = Lattice::ifUpperBoundary(i, j);
			if (iUB == 0) {
				Spin = S_[i][j];
			} else {
				coord = Lattice::helpBoundaryCondition(i, j, iUB);
				Spin = S_[coord[0]][coord[1]];
			}
			*outputFileSpin << i << "\t" << j << "\t" << Spin << endl;
		}
	}

	outputFileSpin->close();
	delete outputFileSpin;
}

// Helper functions
vector<int> SpinLattice::Axial_to_cube(int q, int r)
{
	vector<int> cube(3);
    cube[X] = q - (N_ - Deg_);
    cube[Z] = r - (N_ - Deg_);
    cube[Y] = -cube[X] - cube[Z];
    return cube;
}

vector<int> SpinLattice::Cube_to_axial(int x, int y, int z)
{
	vector<int> ax(2);
	ax[0] = x + (N_ - Deg_);
	ax[1] = z + (N_ - Deg_);
    return ax;
}

// Getters and Setters
double SpinLattice::get_Spin_ax(int q, int r)
{
	return S_[q][r];
}

double SpinLattice::get_Spin_cube(int x, int y, int z)
{
	vector<int> ax = SpinLattice::Cube_to_axial(x, y, z);
	return S_[ax[Q]][ax[R]];
}

void SpinLattice::set_Spin_ax(int q, int r, double val) {
	S_[q][r] = val;
}

void SpinLattice::set_Spin_cube(int x, int y, int z, double val) {
	vector<int> ax = SpinLattice::Cube_to_axial(x, y, z);
	S_[ax[Q]][ax[R]] = val;
}
