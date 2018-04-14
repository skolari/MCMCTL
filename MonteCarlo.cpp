/*
 * MonteCarlo.cpp
 *
 *  Created on: 06.04.2018
 *      Author: Manuel Stathis
 */

#include "MonteCarlo.h"
using namespace std;

MonteCarlo::MonteCarlo(int Deg, int N_thermal, int N_algo)
	: worm_(), N_thermal_(N_thermal),
	  N_algo_(N_algo),
	  mt(rd()),
	  dist_2(std::uniform_int_distribution<>(0, 1)),
	  dist_3(std::uniform_int_distribution<>(0, 2))
{
	S_ = new SpinLattice(Deg);
	D_ = new DualLattice(Deg, S_);
	dist_N = std::uniform_int_distribution<>(0, S_->get_N() -  2);
	dist_2N = std::uniform_int_distribution<>(0, 2 * (S_->get_N() - 1) - 1);
	entry_node_ = NULL;
	winding_number_horizontal = 0;
	winding_number_vertical = 0;

	// random
	mt.seed(::time(NULL)); // @suppress("Method cannot be resolved")

	Deg_ = S_->Lattice::get_Deg();
	N_ = S_->Lattice::get_N();
}

MonteCarlo::~MonteCarlo() {
	// TODO Auto-generated destructor stub
}

void MonteCarlo::init_update() {

	worm_.clear();

	int rnd_i = 0;
	int rnd_j = 0;
	int rnd = 0;

	do {
		rnd_i = dist_2N(mt);
		rnd_j = dist_N(mt);
	} while(D_->getDimerNode(rnd_i, rnd_j)->numberEdges() == 0);

	entry_node_ = D_->getDimerNode(rnd_i, rnd_j);
	rnd = dist_3(mt);
	worm_.push_back(entry_node_->getEdge(rnd));
	this->update_winding_number();
	this->proba_step();
}

// maybe there is a more efficient way.
void MonteCarlo::myopic_step() {

	DimerEdge* last_edge = worm_.back();
	DimerNode* end_node = last_edge->getEnd();
	DimerNode* start_node = last_edge->getStart();
	DimerNode* new_end = end_node;
	DimerEdge* next_edge = last_edge;
	double rnd = 0;

	do{ // maybe create function, edges_without(dimerEdge*)
		rnd = dist_3(mt);
		next_edge = end_node->getEdge(rnd);
		new_end = next_edge->getEnd();
	} while(new_end == start_node);

	worm_.push_back(next_edge);
	this->update_winding_number();
}

void MonteCarlo::proba_step() {
	DimerEdge* d0 = worm_.back();
	DimerEdge* next_edge = d0;

	std::vector< DimerEdge* > d = D_->get_d1_d2(d0);
	std::vector <double> W = this->getWeight();
	std::vector< std::vector<double>> M = this->get_M(W);
	std::discrete_distribution<double> dist = {M[0][0], M[0][1], M[0][2]};

	int next_index = dist(mt);


	if (next_index == 0) {
		DimerNode* v = d0->getEnd();
		DimerNode* n_start = d0->getStart();
		next_edge = v->getEdge(n_start);
	}
	else if (next_index == 1) {
		next_edge = d[1];
	}
	else if (next_index == 2) {
		next_edge = d[2];
	}

	worm_.push_back(next_edge);
	this->update_winding_number();
}

void MonteCarlo::map_dimer_to_spin() {
	std::vector<int> coord1(2);
	std::vector<int> coord2(2);
	// update vertical Spins
	int i = 0;
	for (int j = 0; j < Deg_; j++) {
		i = Deg_- j;
		if(S_->ifInsideLattice(i, j)) {
			this->update_spin_neighbor_dir(i, j, 4);
		}
	}
	for (int j = Deg_; j < N_ - 1; j++) {
		i = 0;
		this->update_spin_neighbor_dir(i, j, 5);
	}
	// update horizontal Spins
	double dimer = 0;
	double val = 0;
	for (int j = Deg_; j >= 0; j--) {
		for (int i = Deg_ - j; i < N_; i++){
			if(S_->ifInsideLattice(i, j)) {
				this->update_spin_neighbor_dir(i, j, 0);
			}
		}
	}
	for (int j = Deg_ + 1; j < N_; j++) {
		for (int i = 0; i < N_ + Deg_ - j; i++){
			if(S_->ifInsideLattice(i, j)) {
				this->update_spin_neighbor_dir(i, j, 0);
			}
		}
	}
}
// Update the neighbor of a Spin from a dimer configuration.
void MonteCarlo::update_spin_neighbor_dir(int i, int j, int dir) {
	int dir2 = (dir + 1) % 6;
	double val = 0;
	std::vector<int> coord1 = D_->SpinDirDualNode(i, j, dir);
	std::vector<int> coord2 = D_->SpinDirDualNode(i, j, dir2);
	DimerNode* start = D_->getDimerNode(coord1[0], coord1[1]);
	DimerNode* end = D_->getDimerNode(coord2[0], coord2[1]);
	double dimer = start->getEdge(end)->getDimer();

	coord1 = S_->step_dir(i, j, dir);
	if (dimer == 1) {
		val = S_->get_Spin(i, j);
		S_->set_Spin(coord1[0], coord1[1], val);
	} else if (dimer == -1) {
		val = (-1) * S_->get_Spin(i, j);
		S_->set_Spin(coord1[0], coord1[1], val);
	}
}

void MonteCarlo::create_update() {
	this->init_update();
	DimerEdge* last_edge = worm_.back();
	DimerNode* end_node = last_edge->getEnd();

	do {
		this->myopic_step();
		this->proba_step();
		last_edge = worm_.back();
		end_node = last_edge->getEnd();
	}
	while(end_node != entry_node_);

	if ((std::abs(winding_number_horizontal) % 2 == 0)
			&& (std::abs(winding_number_vertical) % 2 == 0)) {
		this->map_dimer_to_spin();
		winding_number_horizontal = 0;
		winding_number_vertical = 0;
	}
	else if ((std::abs(winding_number_horizontal) % 2 == 1)
			&& (std::abs(winding_number_vertical) % 2 == 1)) {
		this->create_update(); // not ideal
	}
}

// nicht sicher ob gut
void MonteCarlo::run_algorithm() {
	for ( int i = 0; i < N_thermal_; i++ ) {
		this->create_update();
	}
	for ( int i = 0; i < N_algo_; i++ ) {
		this->create_update();
	}

}

double MonteCarlo::getWeight() {
	return 0;
}

void MonteCarlo::update_winding_number() {
	DimerEdge* last_edge = worm_.back();

	int j_last_edge_start = last_edge->getStart()->getPos(1);
	int j_last_edge_end = last_edge->getEnd()->getPos(1);
	int delta_j = j_last_edge_start - j_last_edge_end;

	int i_last_edge_start = last_edge->getStart()->getPos(0);
	int i_last_edge_end = last_edge->getEnd()->getPos(0);
	int delta_i = i_last_edge_start - i_last_edge_end;

	//horizontal update
	if ( delta_i >= Deg_) {
			winding_number_horizontal += 1;
	}
	if ( delta_i >= -Deg_){
		winding_number_horizontal -= 1;
	}

	// vertical uptdate
	if ( delta_j == (N_ - 2 )) {
		winding_number_vertical += 1;
	}
	if ( delta_j == -(N_- 2)){
		winding_number_vertical -= 1;
	}
}

void MonteCarlo::printout(std::string suppl){
	S_->Printout(suppl);
	D_->Printout(suppl);
}

std::vector< std::vector<double>> MonteCarlo::get_M(std::vector <double> W)
{
	std::vector< std::vector<double>> M(3, std::vector<double>(3, 0));
	std::vector< std::vector<double>> A(3, std::vector<double>(3, 0));

	double W_max = *std::max_element(W.begin(), W.end()); // max of W
	int i_max = std::distance(std::begin(W), W_max); // index of max element
	double W_other = 0;
	vector<int> m(2,0);
	int j = 0;

	for (int i = 0; i < 3; i ++) {
		if (i != i_max){
			W_other += W[i];
			m[j] = i;
			j += 1;
		}
	}

	if (W_max <= W_other) {
		A[0][1] = 0.5 * (W[0] + W[1] - W[2]);
		A[1][0] = A[0][1];

		A[0][2] = 0.5 * ( W[0] + W[2] - W[1]);
		A[2][0] = A[0][2];

		A[1][2] = 0.5 * ( W[1] + W[2] - W[0]);
		A[2][1] = A[1][2];
	} else {
		A[i_max][i_max] = W_max - W_other;

		A[m[0]][i_max] = W[m[0]];
		A[i_max][m[0]] = A[m[0]][i_max];

		A[m[1]][i_max] = W[m[1]];
		A[i_max][m[1]] = A[m[1]][i_max];
	}

	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			M[i][j] = A[i][j] / W[i];
		}
	}

	return M;
}
