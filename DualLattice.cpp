/*
 * DualLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#include "DualLattice.h"

#define I 0
#define J 1

using namespace std;
/*
static double deltaSpintoDimer(double Sij, double Skl) {
	double delta = abs(Sij - Skl);
	if (delta == 0) return 1;
	else if (delta == 2) return -1;
	else return 0;
}
*/
DualLattice::DualLattice(int Deg, SpinLattice* S)
	: Lattice(Deg), S_(S), Dual_(2 * (N_ - 1), vector<DimerNode*>(N_ - 1, NULL))
{
	int i_len = 2 * (N_ - 1);
	int j_len = N_ - 1;

	for ( int j = 0; j < j_len; j++ ) {
		for ( int i = 0; i < i_len; i++ ) {
			Dual_[i][j] = new DimerNode(i, j);
		}
	}

	vector<int> coord(2, 0);
	vector<int> coord2(2, 0);
	int dir2 = 0;
	for ( int j = 0; j < N_; j++ ) {
		for ( int i = 0; i < N_; i++ ) {
			if(S->ifInsideLattice(i, j)) {
				Spin* Spin_right = S->get_Spin_pointer(i, j);
				for (int dir = 0; dir < 6; dir++) {

					Spin* Spin_left = Spin_right->getNeighbor(dir);

					coord = SpinDirDualNode(i, j ,dir);
					DimerNode* start = Dual_[coord[0]][coord[1]];

					dir2 = (dir + 1) % 6;
					coord2 = SpinDirDualNode(i, j ,dir2);
					DimerNode* end = Dual_[coord2[0]][coord2[1]];

					DimerEdge* edge = new DimerEdge(start, end, Spin_right, Spin_left);
					Dual_[coord[0]][coord[1]]->addEdge(edge);

				}
			}
		}
	}
	//  Entry_Sites(N_ * N_, NULL);
	//NDadj_ = i_max * j_max;

	//Dual_adj_ = From_Spin_to_Dual(S);
}

DualLattice::~DualLattice() {
	// TODO Auto-generated destructor stub
}


vector<int> DualLattice::fix_bc(int i, int j) const{

	if ( j == -1 && i >= 2 * Deg_) {
		i = i - 2 * Deg_;
		j = N_ - 2;
	}

	if (i == -1 && j >= Deg_) {
		i = 2 * (N_ - 1) - 1;
		j = j - Deg_;

	}

	if (i == -2 && j >= Deg_) {
		i = 2 * (N_ - 1) - 2;
		j = j - Deg_;

	}

	if (i % 2 == 0) {
		if (i / 2 + j == Deg_ - 1) {
			i = i + 2 * Deg_;
			j = j + Deg_;
		}
	} else {
		if ((i + 1)/ 2 + j == Deg_ - 1) {
			i =  i + 2 * Deg_;
			j = j + Deg_;
		}
	}

	vector<int> coord = {i, j};
	return coord;
}

vector<int> DualLattice::SpinDirDualNode(int i, int j, int dir) const
{
	assert( dir < 6 && dir >= 0);
	int k = i;
	int l = j;

	if (dir == 0) {
		k = 2 * i;
		l = j;
	} else if ( dir == 1) {
		k = 2 * i + 1;
		l = j - 1;
	} else if ( dir == 2) {
		k = 2 * i;
		l = j - 1;
	} else if ( dir == 3) {
		k = 2 * i - 1;
		l = j - 1;
	} else if ( dir == 4) {
		k = 2 * i - 2;
		l = j;
 	} else if ( dir == 5 ) {
 		k = 2 * i - 1;
 		l = j;
 	}

	vector<int> coord = this->fix_bc(k, l);
	return coord;
}

/*
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
	for (int qq = -Deg_; qq <= Deg_; qq++) { // int q = -map_radius; q <= map_radius; q++
		int r1 = max(-Deg_, - qq - Deg_); // int r1 = max(-map_radius, -q - map_radius);
		int r2 = min(Deg_, -qq + Deg_); // int r2 = min(map_radius, -q + map_radius);
    	for (int rr = r1; rr <= r2; rr++) { // for (int r = r1; r <= r2; r++)
    		q = qq + Deg_;
    		r = rr + Deg_;
    		if ((q < N_ - 1) && (r < N_ - 1) && (qq < Deg_)) {
				Spin_ref = S->SpinLattice::get_Spin(q, r);
				k = this->getDadjInd(q, r);

    			for (int dir = 1; dir < 4; dir++) {
    				step = S->SpinLattice::step_dir(q, r, dir);
    				cout << q << "<q,r>" << r << endl;
        			cout << step[I] << " annd "<< step[J] <<" dir: " <<dir <<  endl;
    				Spin_neigh = S->SpinLattice::get_Spin(step[I], step[J]);

    				dimer = deltaSpintoDimer(Spin_ref, Spin_neigh);
    				l = this->getDadjInd(step[I], step[J]);
    				Dual[k][l] = dimer;
    				Dual[l][k] = dimer;
    			}
    		}
		}
	}
	return Dual;
}
*/

vector< vector<double> > DualLattice::getDadj() const{
	vector< vector<double> > Dadj(2 * (N_ - 1) * (N_ - 1), vector<double>(2 * (N_ - 1) * (N_ - 1), 0));

	vector<int> coord(2, 0);
	vector<int> coord2(2, 0);
	int dir2 = 0;
	double dimer = 0;
	int k = 0;
	int l = 0;
	for (int j = 0; j < N_; ++j) {
		for (int i = 0; i < N_; ++i) {
			if(S_->ifInsideLattice(i, j)) {
				for( int dir = 0; dir < 6; dir++ ){

					dir2 = (dir + 1) % 6;
					coord = SpinDirDualNode(i, j ,dir);
					coord2 = SpinDirDualNode(i, j ,dir2);
					DimerNode* end = Dual_[coord2[0]][coord2[1]];
					k = this->getDadjInd(coord[0], coord[1]);
					l = this->getDadjInd(coord2[0], coord2[1]);
					Dadj[k][l] = Dual_[coord[0]][coord[1]]->getEdge(end)->getDimer();

				}
			}
		}
	}
	return Dadj;
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

	vector< vector<double> > Dadj = this->getDadj();
	int i_len = Dadj.size();
	int j_len = Dadj[1].size();

	for (int j = 0; j < j_len; ++j) {
		for (int i = 0; i < i_len; ++i) {
			*outputFileSpin << Dadj[i][j] << "\t";
		}
		*outputFileSpin << endl;
	}

	outputFileSpin->close();
	delete outputFileSpin;
}

