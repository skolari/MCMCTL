/*
 * DualLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

using namespace std;


#include "DualLattice.h"

DualLattice::DualLattice(int Deg, SpinLattice* S)
	: Lattice(Deg)
{
	int i_max = 2 * N_;
	int j_max = N_;
	NDadj_ = i_max * j_max;

	// Dual_(i_max, vector<node*>(j_max, NULL));
	//  Entry_Sites(N_ * N_, NULL);
	Dual_adj_ = From_Spin_to_Dual(S);
}

DualLattice::~DualLattice() {
	// TODO Auto-generated destructor stub
}

vector<vector<double*>> DualLattice::From_Spin_to_Dual(SpinLattice* S) {
	vector<vector<double*>> Dual(NDadj_, vector<double*>(NDadj_));
	double Spin_ref = 0;
	double Spin_neigh = 0;
	for (int q = 0; q < N_ - 1; q++) {
		for(int r = 0; r < N_ - 1; r++) {
			Spin_ref = S->SpinLattice::get_Spin_ax(q, r);
			Spin_neigh = S->SpinLattice::get_Spin_ax(q + 1, r);

			/**
			 * var results = [new Cube(0, 0, 0)];
        for (var k = spiral? 1 : N; k <= N; k++) {
            var H = Cube.scale(Cube.direction(4), k);
            for (var i = 0; i < 6; i++) {
                for (var j = 0; j < k; j++) {
                    results.push(H);
                    H = Cube.neighbor(H, i);
                }
            }
        }
        for (int q = -map_radius; q <= map_radius; q++) {
    		int r1 = max(-map_radius, -q - map_radius);
    		int r2 = min(map_radius, -q + map_radius);
    	for (int r = r1; r <= r2; r++) {
        	map.insert(Hex(q, r, -q-r));
    }
}
			 *
			 */
		}
	}
	return Dual;
}


/**
vector< vector<double> > Lattice::fromSpinToDimerAdj(vector< vector<double> > S)
{
	vector< vector<double> > D(NDadj_, vector<double>(NDadj_, 0.0));
	double delta = 0;
	int k = 0; // (k, l) of Dadj_
	int l = 0; // (k, l) of Dadj_
	int ii = 0; // index i of D(i,j)
	int jj = 0; // index j of D(i,j)
	vector <int> coord1(2);
	vector <int> coord2(2);
	for(int j = 0; j < N_ - 1; ++j) {
		for(int i = 0; i < N_ - 1; ++i)  {
			if (S[i][j] != 0) {
				k = Lattice::getDadjInd(2 * i, j);

				ii = 2 * i - 1;
				jj = j + 1;
				l = Lattice::getDadjInd(ii, jj);
				D[k][l] = deltaSpintoDimer(S[i][j], S[i][j + 1]);
				D[l][k] = D[k][l];

				ii = 2 * i + 1;
				jj = j - 1;
				l = Lattice::getDadjInd(ii, jj);
				D[k][l] = deltaSpintoDimer(S[i][j], S[i + 1][j]);
				D[l][k] = D[k][l];

				ii = 2 * i + 1;
				jj = j;
				l = Lattice::getDadjInd(ii, jj);
				D[k][l] = deltaSpintoDimer(Lattice::getS_ij(i + 1, j), Lattice::getS_ij(i, j + 1));
				D[l][k] = D[k][l];
			}
		}
	}
	return D;
}

/**
