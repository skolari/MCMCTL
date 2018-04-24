/*
 * MonteCarlo.h
 *
 *  Created on: 06.04.2018
 *      Author: Manuel Stathis
 */

#ifndef MONTECARLO_H_
#define MONTECARLO_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "SpinLattice.h"
#include "DualLattice.h"
#include "DimerEdge.h"
#include "DimerNode.h"


class MonteCarlo {
public:
	MonteCarlo(int Deg, int N_thermal, int N_algo = 0, double J1 = 0, double J2 = 0, double J3 = 0);
	virtual ~MonteCarlo();

	// Algorithm methods
	void init_update();
	void myopic_step();
	void proba_step();
	void map_dimer_to_spin();
	void create_update();
	void run_algorithm();
	void run_parallel_step(int N_temp);


	// Printout
	void printout(std::string suppl);

	// getters and setters
	inline DimerNode* get_entry_node() const {
		return entry_node_;
	}

	inline int get_winding_number_vertical() const {
		return winding_number_vertical;
	}

	inline int get_winding_number_horizontal() const {
			return winding_number_horizontal;
	}

	inline double get_energy() const {
		S_->update_Energy(); // should update after every probastep...
		return S_->get_Energy();
	}

	// Probastep matrixes
	std::vector< std::vector<double>> get_M(std::vector <double> W);

	void update_spin_neighbor_dir(int i, int j, int dir);
	void update_winding_number();
private:
	SpinLattice* S_;
	DualLattice* D_;

	DimerNode* entry_node_;
	std::vector<DimerEdge*> worm_;

	int winding_number_horizontal;
	int winding_number_vertical;
	// Deg_, N_
	int Deg_;
	int N_;
	// Algorithm
	int N_thermal_;
	int N_algo_;

	// Random
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dist_2; // chooses randomly 0 to 1
	std::uniform_int_distribution<int> dist_3; // chooses randomly 0 to 2
	std::uniform_int_distribution<int> dist_N; // chooses randomly 0 to N-1, N size of spin lattice.
	std::uniform_int_distribution<int> dist_2N; // chooses randomly 0 to N-1, N size of spin lattice.


};

#endif /* MONTECARLO_H_ */
