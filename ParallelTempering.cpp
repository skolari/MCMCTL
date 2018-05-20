/*
 * ParallelTempering.cpp
 *
 *  Created on: 24.04.2018
 *      Author: Manuel Stathis
 */

#include "ParallelTempering.h"

using namespace std;

ParallelTempering::ParallelTempering(Random* Rnd, int Deg, int N_simul, int N_thermal, int N_algo, int N_temp, int N_measure, bool Dipolar,
		double J1, double J2, double J3, double J5, double delta_J,
		double T_start, double T_end)
	: Rnd_(Rnd), Deg_(Deg), N_simul_(N_simul), N_thermal_(N_thermal), N_algo_(N_algo), N_temp_(N_temp), N_measure_(N_measure), J1_const_(J1), J2_const_(J2), J3_const_(J3), J5_const_(J5),
	  beta_(N_simul, 0), Simulations_(N_simul, NULL)
{

	for (int i = 0; i < N_simul; i++) {
		beta_[i] = 1 / (T_start + i * (T_end - T_start) / N_simul);
	}

	for (int i = 0; i < N_simul; i++) {
		Simulations_[i] = new MonteCarlo(Rnd, Deg_, N_thermal, N_algo, Dipolar, J1_const_, J2_const_, J3_const_, J5_const_, delta_J, beta_[i]);
	}
}


ParallelTempering::~ParallelTempering() {
	// TODO Auto-generated destructor stub
}

/**
 * Runs the algorithm steps.
 * First the N_thremal_ times thermalisation and then
 * N_algo_ times the algogrithm step where all N_measure_ steps the energy is mesured.
 */
void ParallelTempering::run() {
	// thermalisation
	for ( int i = 0; i < N_thermal_; i++ ) {
		if (i % 100 == 0) {
			std::cout << i << " out of " << N_thermal_ << " thermal steps done." << std::endl;
		}
		this->algorithm_step();
	}

	// algorithm
	for ( int i = 0; i < N_algo_; i++ ) {
		if (i % 100 == 0)
			std::cout << i << " out of " << N_algo_ << " algo steps done." << std::endl;
		this->algorithm_step();
		if (i % N_measure_ == 0) {
			this->measure_energy();
		}
	}
}

/**
 * This is runs in parallel all MonteCarlo systems with different Temperatures.
 * In the end the parallel tempering will be executed.
 */
void ParallelTempering::algorithm_step() {

	omp_set_num_threads(N_simul_);

	#pragma omp parallel for
	for(int i = 0; i < N_simul_; i++) {
		Simulations_[i]->run_parallel_step(N_temp_);
	}

	for ( int i = 0 ; i < N_simul_ - 1; i++) {
		this->tempering_switch(i, i + 1);
	}
}

/**
 * If the energy of two systems is not raising the two systems are exchanged and is exchanged too.
 * Else the systems are swaped with a certain probability.
 * @param i index of the ith MonteCarlo system in Simulations_
 * @param j index of the jth MonteCarlo system in Simulations_
 */
void ParallelTempering::tempering_switch(int i, int j) {
	MonteCarlo* Mi = Simulations_[i];
	MonteCarlo* Mj = Simulations_[j];
	double Ei = Mi->get_energy();
	double Ej = Mj->get_energy();
	double beta_i = Simulations_[i]->get_S()->get_Beta();
	double beta_j = Simulations_[j]->get_S()->get_Beta();
	double ref = std::exp((beta_i - beta_j) * (Ei - Ej));

	if (ref > 1) {
		this->J_swap(i, j);
	} else {
		double rnd = Rnd_->dist();
		if (rnd < ref) {
			this->J_swap(i, j);
		}
	}
}

/**
 * Swap the temperature of two systems.
 * @param i index of the ith MonteCarlo system in Simulations_
 * @param j index of the jth MonteCarlo system in Simulations_
 */
void ParallelTempering::J_swap(int i, int j) {
	double Beta_temp = Simulations_[i]->get_S()->get_Beta();
	Simulations_[i]->get_S()->set_Beta(Simulations_[j]->get_S()->get_Beta());
	Simulations_[j]->get_S()->set_Beta(Beta_temp);

	vector<double> energy_measures = Simulations_[i]->get_energy_measures_();
	Simulations_[i]->set_energy_measures_(Simulations_[j]->get_energy_measures_());
	Simulations_[j]->set_energy_measures_(energy_measures);

	vector<double> magnetisation_measures = Simulations_[i]->get_magnetisation_measures_();
	Simulations_[i]->set_magnetisation_measures_(Simulations_[j]->get_magnetisation_measures_());
	Simulations_[j]->set_magnetisation_measures_(magnetisation_measures);

	vector<double> nstring_measures = Simulations_[i]->get_nstring_measures_();
	Simulations_[i]->set_nstring_measures_(Simulations_[j]->get_nstring_measures_());
	Simulations_[j]->set_nstring_measures_(nstring_measures);



	std::swap(Simulations_[i],Simulations_[j]);
}


/**
 * Print out the mean energy per spin and Cv and the last spin and dual configuration of the system.
 * @param OutputPath outputpath of the output .dat files.
 */
void ParallelTempering::Printout(std::string OutputPath) {
	std::string s = "";
	std::string OutputPath_new = "";
	for (int i = 0; i < N_simul_; i++) {
		s = std::to_string(i);
		OutputPath_new  = OutputPath + "nr_" + s;
		Simulations_[i]->Printout(OutputPath_new);
	}
	this->Printout_Observables(OutputPath);
}

/**
 * create a printout .dat file where the mean energy and cv of all systems in Simulations_ is saved.
 * @param OutputPath outputpath of the output .dat files.
 */
void ParallelTempering::Printout_Observables(std::string OutputPath) const
{
	string path = OutputPath + "Observables.dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	double E = 0;
	double Cv = 0;
	double beta = 0;
	double Bc = 0;
	double nstring = 0;
	for (int i = 0; i < N_simul_; ++i) {
		beta = Simulations_[i]->get_S()->get_Beta();
		E = Simulations_[i]->first_moment_energy();
		Cv = Simulations_[i]->calculate_cv();
		Bc  = Simulations_[i]->calculate_binder_cumulant();
		nstring = Simulations_[i]->first_moment_nstring();
		*outputFileSpin << std::fixed << std::showpoint << std::setprecision(5) << beta << "\t" << E << "\t" << Cv << "\t" << Bc << "\t" << nstring << endl;
	}
	outputFileSpin->close();
	delete outputFileSpin;
}


/**
 * calls the MonteCarlo::measure_energy() for all systems in Simulations_
 */
void ParallelTempering::measure_energy() {
	for(int i = 0; i < N_simul_; i++) {
		Simulations_[i]->measure();
	}
}

