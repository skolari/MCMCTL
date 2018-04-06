/*
 * MonteCarlo.cpp
 *
 *  Created on: 06.04.2018
 *      Author: Manuel Stathis
 */

#include "MonteCarlo.h"

namespace std {

MonteCarlo::MonteCarlo(int Deg, int N_thermal, int N_algo)
	: worm_(), N_thermal_(N_thermal),
	  N_algo_(N_algo),
	  mt(rd()),
	  dist_2(uniform_int_distribution<>(0, 1)),
	  dist_3(uniform_int_distribution<>(0, 2))
{
	S_ = new SpinLattice(Deg);
	D_ = new DualLattice(Deg, S_);
	dist_N = uniform_int_distribution<>(0, S_->get_N()-  1);
	dist_2N = uniform_int_distribution<>(0, 2 * (S_->get_N()-1));
	entry_node_ = NULL;
	winding_number_ = 0;

	// random
	mt.seed(::time(NULL)); // @suppress("Method cannot be resolved")

	Deg_ = S_->get_Deg();
	N_ = S_->get_N();
}

MonteCarlo::~MonteCarlo() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */

void MonteCarlo::init_update() {

	worm_.clear();

	int rnd_i = 0;
	int rnd_j = 0;
	int rnd = 0;

	do {
		rnd_i = dist_2N(mt);
		rnd_j = dist_N(mt);
	} while(D_->getDimerNode(rnd_i, rnd_j)->getEdges().empty());

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
	} while(new_end != start_node);

	worm_.push_back(next_edge);
	this->update_winding_number();
}

void MonteCarlo::proba_step() {

	this->update_winding_number();
}

void MonteCarlo::map_dimer_to_spin() {
	std::vector<int> coord1(2);
	std::vector<int> coord2(2);
	// update vertical Spins
	int i = 0;
	for (int j = 0; j < Deg_; j++) {
		i = Deg_- j;
		this->update_spin_neighbor_dir(i, j, 4);
	}
	for (int j = Deg_; j < N_; j++) {
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
	double dimer = D_->getDimerNode(coord1[0], coord1[1])->getEdge(D_->getDimerNode(coord1[0], coord1[1]))->getDimer();

	coord1 = S_->get_direction(i, j, dir);
	if (dimer == 1) {
		val = S_->get_Spin(i, j);
		S_->set_Spin(coord1[0], coord1[1], val);
	} else if (dimer == -1) {
		val = (-1) * S_->get_Spin(i, j);
		S_->set_Spin(coord1[0], coord1[1], val);
	}
}

void MonteCarlo::create_update() {
	DimerEdge* last_edge = worm_.back();
	DimerNode* end_node = last_edge->getEnd();
	this->init_update();

	do {
		this->myopic_step();
		this->proba_step();
		last_edge = worm_.back();
		end_node = last_edge->getEnd();
	}
	while(end_node != entry_node_);

	if (std::abs(winding_number_) % 2 == 0) {
		this->map_dimer_to_spin();
		winding_number_ = 0;
	}
	else if (std::abs(winding_number_) % 2 == 1) {
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

	if ( delta_j == (N_ - 2 )) {
		winding_number_ += 1;
	}
	if ( delta_j == -(N_- 2)){
		winding_number_ -= 1;
	}
}
