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
	if (!( dir < 6 && dir >= 0)) cerr << "dir not in range" << endl;
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

void DualLattice::switchDimer(DimerEdge* edge) {
	DimerNode* start = edge->getStart();
	DimerNode* end = edge->getEnd();
	DimerEdge* opposite = end->getEdge(start);
	double dimer = (-1) * edge->getDimer();
	edge->setDimer(dimer);
	opposite->setDimer(dimer);
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

/*
 * get d1 and d2 like in the figure see rakala
 */
vector< DimerEdge* > DualLattice::get_d1_d2(DimerEdge* d0)
{
	DimerNode* e = d0->getStart();
	DimerNode* pc = d0->getEnd();
	DimerEdge* d1 = NULL;
	DimerEdge* d2 = NULL;

	vector <DimerEdge*> edges = pc->getEdges();
	for (auto edge : edges) {
		if (e != edge->getStart()) {
			if ( edge->getSpin_right()  == d0->getSpin_right() ) {
				DimerEdge* d1 = edge;
			}
			else if( edge->getSpin_left()  == d0->getSpin_left() ) {
				DimerEdge* d2 = edge;
			}
			else {
				cerr << "edge is not connected" << endl;
			}
		}
	}
	return {d0, d1, d2};
}

/*
 * get s1 to s11 like in the figure see rakala
 */
vector< DimerEdge* > DualLattice::get_s_dimer(vector<DimerEdge*> d) {
	vector< DimerEdge*> s(12, NULL);

	DimerEdge* edge = NULL;
	// around the first spin
	DimerNode* start = d[1]->getEnd();
	DimerNode* end = d[2]->getEnd();
	Spin* spin_left = d[1]->getSpin_left();

	for (int j = 0; j < 4; j++) {
		int i = 0;
		edge = start->getEdge(i);
		while(edge->getSpin_left() != spin_left) {
			i += 1;
			edge = start->getEdge(i);
		}
		s[j] = edge;
		start = edge->getEnd();
	}
	if(edge->getEnd() != end) {
		cerr << "end ist not the real end" << endl;
	}

	// around the second spin
	DimerNode* start = d[2]->getEnd();
	DimerNode* end = d[0]->getStart();
	Spin* spin_left = d[2]->getSpin_left();

	for (int j = 4; j < 8; j++) {
		int i = 0;
		edge = start->getEdge(i);
		while(edge->getSpin_left() != spin_left) {
			i += 1;
			edge = start->getEdge(i);
		}
		s[j] = edge;
		start = edge->getEnd();
	}
	if(edge->getEnd() != end) {
		cerr << "end ist not the real end" << endl;
	}

	//around the third spin
	DimerNode* start = d[0]->getStart();
	DimerNode* end = d[1]->getEnd();
	Spin* spin_left = d[0]->getSpin_right();

	for (int j = 8; j < 12; j++) {
		int i = 0;
		edge = start->getEdge(i);
		while(edge->getSpin_left() != spin_left) {
			i += 1;
			edge = start->getEdge(i);
		}
		s[j] = edge;
		start = edge->getEnd();
	}
	if(edge->getEnd() != end) {
		cerr << "end ist not the real end" << endl;
	}

	return s;
}

double DualLattice::get_local_weight(vector< DimerEdge* > d,vector< DimerEdge* > s)
{
	double w = 0;
	return w;
}
