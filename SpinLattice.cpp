/*
 * SpinLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#include "SpinLattice.h"
using namespace std;

#define Q 0
#define R 1


SpinLattice::SpinLattice(int Deg)
	: Lattice(Deg), S_(2 * Deg + 1, vector<Spin*>(2 * Deg + 1, NULL))
{
	double rnd = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			S_[i][j] = new Spin(i, j);
			if (this->ifInsideLattice(i, j)) {
				rnd = dist(mt);
				if (rnd < 0.5) {
					S_[i][j]->Spin::setSpin(1);
				}
				else {
					S_[i][j]->Spin::setSpin(-1);
				}
			}
		}
	}
	// Set neighbors
	vector<int> step (2, 0);
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (this->ifInsideLattice(i, j)) {
				for (int dir = 0; dir < 6; dir++) {
					step = SpinLattice::step_dir(i, j, dir);
					S_[i][j]->Spin::addNeighbor(S_[step[0]][step[1]], dir);
				}
			}
		}
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
	vector<int> coord(2, 0);
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			Spin = this->get_Spin(i, j);
			*outputFileSpin << i << "\t" << j << "\t" << Spin << endl;
		}
	}
	outputFileSpin->close();
	delete outputFileSpin;
}


// Getters and Setters


void SpinLattice::set_Spin(Spin* S, double val) {
	S->Spin::setSpin(val);
}

vector<int> SpinLattice::fix_bc(int i, int j) const{
	if (j == N_ - 1  && i < Nc_) {
		i = i + Deg_;
		j = 0;
	}
	if(i == N_ - 1 && j < Nc_) {
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

bool SpinLattice::ifInsideLattice(int i, int j) {
	bool flag = true;
	if (i + j < Deg_) { flag = false; }
	if (i == N_ - 1) { flag = false; }
	if (j == N_ - 1) { flag = false; }
	if (i + j > N_ + Deg_ - 2) { flag = false; }
	return flag;
}
// Clockwise starting from 3 o'clock == 0
vector<int> SpinLattice::step_dir(int i, int j, int d) {
	assert(d < 6 && d >= 0);
	vector<int> step(2);
	if (d == 0) { // before d = 1
		i = i + 1;
		step = this->fix_bc(i, j);
	} else if (d == 1){ // before d = 3
		j = j - 1;
		i = i + 1;
		step = this->fix_bc(i, j);
	} else if (d == 2) { // before d = 2
		j = j - 1;
		step = this->fix_bc(i, j);
	} else if (d == 3) { // before d = -1
		i = i - 1;
		step = this->fix_bc(i, j);
	} else if (d == 4) { // before d = -3
		j = j + 1;
		i = i - 1;
		step = this->fix_bc(i, j);
	} else if (d == 5) { // before d = -2
		j = j + 1;
		step = this->fix_bc(i, j);
	}
	return step;
}
