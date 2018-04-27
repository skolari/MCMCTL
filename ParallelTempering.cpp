/*
 * ParallelTempering.cpp
 *
 *  Created on: 24.04.2018
 *      Author: Manuel Stathis
 */

#include "ParallelTempering.h"

using namespace std;

ParallelTempering::ParallelTempering(int Deg, int N_simul, int N_thermal, int N_algo, int N_temp,
		double J1, double J2, double J3,
		double beta_start, double beta_end)
	: Deg_(Deg), N_simul_(N_simul), N_thermal_(N_thermal), N_algo_(N_algo), N_temp_(N_temp), J1_const_(J1), J2_const_(J2), J3_const_(J3), beta_(N_simul, 0), J1_(N_simul, 0), J2_(N_simul, 0), J3_(N_simul, 0), Simulations_(N_simul, NULL)
{
	// TODO Auto-generated constructor stub
	int N_s = N_simul;

	if (N_simul == 1) {
		N_s = 2;
	}

	for (int i = 0; i < N_simul; i++) {
		beta_[i] = beta_start + (i - 1) * (beta_end - beta_start) / (N_s - 1);
	}
	for (int i = 0; i < N_simul; i++) {
		J1_[i] = beta_[i] * J1_const_;
		J2_[i] = beta_[i] * J2_const_;
		J3_[i] = beta_[i] * J3_const_;
	}

	for (int i = 0; i < N_simul; i++) {
		Simulations_[i] = new MonteCarlo(Deg_, N_thermal, N_algo, J1_[i], J2_[i], J3_[i]);
	}
}

ParallelTempering::~ParallelTempering() {
	// TODO Auto-generated destructor stub
}

void ParallelTempering::run() {

	// thermalisation
	for ( int i = 0; i < N_thermal_; i++ ) {
		this->algorithm_step();
	}

	// algorithm
	for ( int i = 0; i < N_algo_; i++ ) {
		this->algorithm_step();
	}
}

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

void ParallelTempering::tempering_switch(int i, int j) {
	MonteCarlo* M1 = Simulations_[i];
	MonteCarlo* M2 = Simulations_[j];
	double E1 = M1->get_energy();
	double E2 = M2->get_energy();

	if (E1 > E2) {
		this->J_swap(i, j);
	} else {
		// use random device,, update all constructors with random device.
	}

}

void ParallelTempering::J_swap(int i, int j) {
	double J_temp;

	for ( int k = 0; k < 3; k++) {
		J_temp = Simulations_[i]->get_Ji(k);
		Simulations_[i]->set_Ji(k, Simulations_[j]->get_Ji(k));
		Simulations_[j]->set_Ji(k, J_temp);
	}

	std::swap(Simulations_[i],Simulations_[j]);
}

void ParallelTempering::printout(std::string OutputPath) {
	std::string s = "";
	std::string OutputPath_new = "";
	omp_set_num_threads(N_simul_);

	#pragma omp parallel for
	for (int i = 0; i < N_simul_; i++) {
		s = std::to_string(i);
		OutputPath_new  = OutputPath + s;
		Simulations_[i]->printout(OutputPath_new);
	}
	this->PrintoutEnergy(OutputPath);
}

void ParallelTempering::PrintoutEnergy(std::string OutputPath) const
{
	string path = OutputPath + "_Energy.dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	double E = 0;
	for (int i = 0; i < N_simul_; ++i) {
		E = Simulations_[i]->get_S()->get_Energy();
		*outputFileSpin << beta_[i] << "\t" << E << endl;
	}
	outputFileSpin->close();
	delete outputFileSpin;
}
