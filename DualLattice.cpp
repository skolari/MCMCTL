/*
 * DualLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#include "DualLattice.h"

#define Q 0
#define R 1

using namespace std;

static double deltaSpintoDimer(double Sij, double Skl) {
	double delta = abs(Sij - Skl);
	if (delta == 0) return 1;
	else if (delta == 2) return -1;
	else return 0;
}

DualLattice::DualLattice(int Deg, SpinLattice* S)
	: Lattice(Deg)
{
	int i_max = 2 * (N_ - 1);
	int j_max = N_ - 1;
	NDadj_ = i_max * j_max;

	// Dual_(i_max, vector<node*>(j_max, NULL));
	//  Entry_Sites(N_ * N_, NULL);
	Dual_adj_ = From_Spin_to_Dual(S);
}

DualLattice::~DualLattice() {
	// TODO Auto-generated destructor stub
}

vector<vector<double>> DualLattice::From_Spin_to_Dual(SpinLattice* S) {
	vector<vector<double>> Dual(NDadj_, vector<double>(NDadj_));
	double Spin_ref = 0;
	double Spin_neigh = 0;
	int r = 0;
	int q = 0;
	int k = 0;
	int l = 0;
	double dimer = 0;
	vector<int> step(2);
	step = S->SpinLattice::step_dir(3, 0, 1);
	cout << "q = " <<step[Q]<< ", r = "<< step[R]<<endl;
	for (int qq = -Deg_; qq <= Deg_; qq++) { // int q = -map_radius; q <= map_radius; q++
		int r1 = max(-Deg_, - qq - Deg_); // int r1 = max(-map_radius, -q - map_radius);
		int r2 = min(Deg_, -qq + Deg_); // int r2 = min(map_radius, -q + map_radius);
    	for (int rr = r1; rr <= r2; rr++) { // for (int r = r1; r <= r2; r++)
    		r = rr + Deg_;
    		q = qq + Deg_;
    		if ((q < N_ - 1) && (r < N_ - 1) && (qq < Deg_)) {

				Spin_ref = S->SpinLattice::get_Spin_ax(q, r);
				k = this->getDadjInd(q, r);

    			for (int dir = 1; dir < 4; dir++) {
    				step = S->SpinLattice::step_dir(q, r, dir);
    				Spin_neigh = S->SpinLattice::get_Spin_ax(step[Q], step[R]);

    				dimer = deltaSpintoDimer(Spin_ref, Spin_neigh);
    				l = this->getDadjInd(step[Q], step[R]);
    				Dual[k][l] = dimer;
    				Dual[l][k] = dimer;
    			}
    		}
		}
	}
	return Dual;
}

// (i, j) indices of Dimer Lattice, returns coordinates in Ajd matrix.
inline int DualLattice::getDadjInd(int i, int j) const
{
	return N_ * j + i;
}


void DualLattice::Printout(string suppl) const
{
	string path = "./Outputfiles/DimerAdj" + suppl + ".dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	for (int i = 0; i < NDadj_; ++i) {
		for (int j = 0; j < NDadj_; ++j) {
			*outputFileSpin << Dual_adj_[i][j] << "\t";
		}
		*outputFileSpin << endl;
	}

	outputFileSpin->close();
	delete outputFileSpin;
}

