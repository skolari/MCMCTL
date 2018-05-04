/*
 * SpinLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#include "SpinLattice.h"
using namespace std;


SpinLattice::SpinLattice(Random* Rnd, int Deg, double J1, double J2, double J3, double Beta)
	: Lattice(Rnd, Deg), S_(2 * Deg + 1, vector<Spin*>(2 * Deg + 1, NULL)), J1_(J1), J2_(J2), J3_(J3), Beta_(Beta)
{
	double rnd = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			S_[i][j] = new Spin(i, j);
			if (this->ifInsideLattice(i, j)) {
				rnd = Rnd_->dist();
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
	Number_spin_ = this->SpinLattice::number_spin();
	Energy_ = this->calculate_Energy();

}

SpinLattice::~SpinLattice()
{
	// TODO Auto-generated constructor stub
}

void SpinLattice::Printout(string outputPath) const
{
	string path = outputPath + "_Spin.dat";

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

void SpinLattice::set_Spin(int i, int j, double val){
	Spin* S = this->get_Spin_pointer(i, j);
	this->set_Spin(S, val);
}


vector<int> SpinLattice::fix_bc(int i, int j) const{
	if (j == N_ - 1  && i < Nc_) {
		i = i + Deg_;
		j = 0;
	}
	if(i == N_ - 1 && j == Deg_) {
		i = Deg_;
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
	if(!(d < 6 && d >= 0)) cerr << "dir index not in range" << endl;
	vector<int> step(2, 0);
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

double SpinLattice::calculate_Energy() {
	double Energy = 0;
	Spin* Spin_origin = NULL;
	Spin* Spin_neigh = NULL;
	double S_o = 0;
	double S_n = 0;
	int dir2 = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (this->ifInsideLattice(i, j)) {
				Spin_origin = this->SpinLattice::get_Spin_pointer(i, j);
				S_o =  Spin_origin->getSpin();
				// an other possibility is to loop over all dir and divide the final energy by 2
				for (int dir = 0; dir < 3; dir++) {
					// nearest neighbor
					Spin_neigh = Spin_origin->getNeighbor(dir);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J1_ * S_o * S_n;

					// next nearest neighbor
					dir2 = (dir + 1) % 6;
					Spin_neigh = Spin_origin->getNeighbor(dir)->getNeighbor(dir2);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J2_ * S_o * S_n;

					// next next nearest neighbor
					Spin_neigh = Spin_origin->getNeighbor(dir)->getNeighbor(dir);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J3_ * S_o * S_n;
				}
			}
		}
	}
	return Energy;
}

Spin* SpinLattice::get_Spin_pointer(int i, int j) const
{
	return S_[i][j];
}


void SpinLattice::update_Energy()
{
	Energy_ = this->calculate_Energy();
}

void SpinLattice::update_Energy(double val)
{
	Energy_ = Energy_ + val;
}

double SpinLattice::get_magnetisation_per_spin() {
	double M  = 0;
	for (int i = 0; i < N_; i++) {
		for (int j = 0; j < N_; j++) {
			if (this->ifInsideLattice(i, j)) {
				M += this->get_Spin(i, j);
			}
		}
	}
	M = M / Number_spin_;
	return M;
}

// can be optimized
int SpinLattice::number_spin() {
	int count = 0;
	for (int i = 0; i < N_; i++) {
		for (int j = 0; j < N_; j++) {
			if (this->ifInsideLattice(i, j)) {
				count += 1;
			}
		}
	}
	return count;
}

double SpinLattice::get_energy_per_spin() {
	this->update_Energy();
	double E = this->get_Energy();
	return E / Number_spin_;
}
