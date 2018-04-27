/*
 * ParallelTempering.cpp
 *
 *  Created on: 24.04.2018
 *      Author: Manuel Stathis
 */

#include "ParallelTempering.h"

using namespace std;

ParallelTempering::ParallelTempering(int Deg, int N_simul, int N_thermal, int N_algo, int N_temp, double J1_start, double J2_start, double J3_start, double J1_end, double J2_end, double J3_end)
	: Deg_(Deg), N_simul_(N_simul), N_thermal_(N_thermal), N_algo_(N_algo), N_temp_(N_temp), J1_(N_simul, 0), J2_(N_simul, 0), J3_(N_simul, 0), Simulations_(N_simul, NULL)
{
	// TODO Auto-generated constructor stub
	int N_s = N_simul;

	if (N_simul == 1) {
		N_s = 2;
	}

	for (int i = 0; i < N_simul; i++) {
		J1_[i] = J1_start + (i - 1) * (J1_end - J1_start) / (N_s - 1);
		J2_[i] = J2_start + (i - 1) * (J2_end - J2_start) / (N_s - 1);
		J3_[i] = J3_start + (i - 1) * (J3_end - J3_start) / (N_s - 1);
	}

	double J1, J2, J3;
	for (int i = 0; i < N_simul; i++) {
		J1 = J1_[i];
		J2 = J2_[i];
		J3 = J3_[i];
		Simulations_[i] = new MonteCarlo(Deg_, N_thermal, N_algo, J1, J2, J3);
	}
}

ParallelTempering::~ParallelTempering() {
	// TODO Auto-generated destructor stub
}

void ParallelTempering::run() {

	// thermalisation
	int N = (int) N_thermal_/10;

	for ( int i = 0; i < N; i++ ) {
		this->algorithm_step();
	}

	// algorithm
	N = (int) N_algo_/10;
	for ( int i = 0; i < N; i++ ) {
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
