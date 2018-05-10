/*
 * MonteCarlo.h
 *
 *  Created on: 06.04.2018
 *      Author: Manuel Stathis
 */

#ifndef MONTECARLO_H_
#define MONTECARLO_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <random>

#include "SpinLattice.h"
#include "DualLattice.h"
#include "DimerEdge.h"
#include "DimerNode.h"
#include "Random.h"

class MonteCarlo {
public:
	MonteCarlo(Random* Rnd, int Deg, int N_thermal, int N_algo, double J1, double J2, double J3, double Beta);
	virtual ~MonteCarlo();

	// Algorithm methods
	void init_update();
	void myopic_step();
	void proba_step();
	void map_dimer_to_spin();
	void create_update();
	void run_algorithm_single();
	void run_parallel_step(int N_temp);


	// Printout
	void Printout(std::string OutputPath);

	// getters and setters
	inline DimerNode* get_entry_node() const {
		return entry_node_;
	}

	inline std::vector<double> get_energy_measures_() {
		return energy_measures_;
	}

	inline void set_energy_measures_(std::vector<double> v) {
		energy_measures_  = v;
	}

	inline int get_winding_number_1() const {
		return winding_number_1;
	}

	inline int get_winding_number_2() const {
		return winding_number_2;
	}

	inline double get_energy() const {
		S_->update_Energy(); // should update after every probastep...
		return S_->get_Energy();
	}

	inline SpinLattice* get_S() {
		return S_;
	}

	// Probastep matrixes
	std::vector< std::vector<long double>> get_M(std::vector <long double> W);

	void update_spin_neighbor_dir(int i, int j, int dir);
	void update_winding_number();

	// Mesures
	void measure_energy();

	double first_moment_energy();
	double second_moment_energy();
	double calculate_cv();
	double variance_energy();
	void delete_worm();

	//test function
	void testwindingnumber();

private:
	Random* Rnd_;
	SpinLattice* S_;
	DualLattice* D_;

	DimerNode* entry_node_;
	std::vector<DimerEdge*> worm_;

	int winding_number_1;
	int winding_number_2;

	std::vector<double> energy_measures_;
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
