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
#include "LatticeTopology.h"

class MonteCarlo {
public:
	MonteCarlo(Random* Rnd, int Deg, int N_thermal, int N_algo, bool Dipolar, double J1, double J2, double J3, double J5, double delta_J, double Beta);
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
	void Printout_Fourier(std::string Outputpath);
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

	inline std::vector<double> get_magnetisation_measures_() {
		return magnetisation_measures_;
	}

	inline void set_magnetisation_measures_(std::vector<double> v) {
		magnetisation_measures_  = v;
	}

	inline std::vector<double> get_nstring_measures_() {
		return nstring_measures_;
	}

	inline void set_nstring_measures_(std::vector<double> v) {
		nstring_measures_  = v;
	}

	inline std::vector<double> get_gm_measures_() {
		return gm_measures_;
	}

	inline void set_gm_measures_(std::vector<double> v) {
		gm_measures_  = v;
	}

	inline std::vector<std::vector<std::vector<double>>> get_correlation_measures_() {
		return correlation_measures_;
	}

	inline void set_correlation_measures_(std::vector<std::vector<std::vector<double>>> v) {
		correlation_measures_  = v;
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
	void measure();

	double first_moment_energy();
	double first_moment(std::vector< double > v);
	double second_moment(std::vector< double > v);
	double forth_moment(std::vector< double > v);
	double calculate_cv();
	double calculate_binder_cumulant();
	void calculate_winding_strings();
	double first_moment_nstring();
	std::vector< std::vector <double> > first_moment_correlations();
	void update_winding_i_dir(int index, int i, int j, int dir, bool plus);

	double variance_energy();
	void delete_worm();
	void delete_worm_archive();

	//test function
	void testwindingnumber();

private:
	Random* Rnd_; // pointer to the random engine of the programm
	SpinLattice* S_; // pointer to the spin lattice
	DualLattice* D_; // pointer to the dual lattice

	DimerNode* entry_node_; // the first node of the worm.
	std::vector<DimerEdge*> worm_; // the worm which is on construction
	std::vector<DimerEdge*> worm_archive_; // this collects all closed worms till the map2spin function is called.

	int winding_number_1;
	int winding_number_2;
	std::vector< int > winding_strings_; //winding number which is used to define n_string

	std::vector<double> energy_measures_; // vector which collects the energy measures
	std::vector<double> magnetisation_measures_; // vector which collects the magnetisation measures
	std::vector<double> nstring_measures_; // vector which collects the nstring measures
	std::vector<double> gm_measures_; // vector which collects the gm measures
	std::vector < std::vector<std::vector<double>>> correlation_measures_; // vector which collects the gm measures

	// Deg_, N_
	int Deg_;
	int N_;
	// Algorithm
	int N_thermal_;
	int N_algo_;
};

#endif /* MONTECARLO_H_ */
