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
	: Deg_(Deg), N_simul_(N_simul), N_thermal_(N_thermal), N_algo_(N_algo), N_temp_(N_temp), J1_const_(J1), J2_const_(J2), J3_const_(J3),
	  beta_(N_simul, 0), J1_(N_simul, 0), J2_(N_simul, 0), J3_(N_simul, 0), Simulations_(N_simul, NULL), mt(rd()), dist(uniform_real_distribution<>(0.0, 1.0))
{

	for (int i = 0; i < N_simul; i++) {
		beta_[i] = beta_start + i * (beta_end - beta_start) / N_simul;
	}

	for (int i = 0; i < N_simul; i++) {
		Simulations_[i] = new MonteCarlo(Deg_, N_thermal, N_algo, J1_const_, J2_const_, J3_const_, beta_[i]);
	}
}

ParallelTempering::~ParallelTempering() {
	// TODO Auto-generated destructor stub
}

void ParallelTempering::run() {

	// thermalisation
	for ( int i = 0; i < N_thermal_; i++ ) {
		this->algorithm_step();
		if (i % 10 == 0) {
			std::cout << i << " out of " << N_thermal_ << " thermal steps done." << std::endl;
		}
	}

	// algorithm
	for ( int i = 0; i < N_algo_; i++ ) {
		this->algorithm_step();
		if (i % 10 == 0) {
			this->mesure_energy();
			std::cout << i << " out of " << N_algo_ << " algo steps done." << std::endl;
		}
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
	double ref = std::exp((beta_[i] - beta_[j]) * (E1 - E2));
	if (ref > 1) {
		this->J_swap(i, j);
	} else {
		double rnd = dist(mt);
		if (rnd < ref) {
			this->J_swap(i, j);
		}
	}

}

void ParallelTempering::J_swap(int i, int j) {
	double J_temp;

	for ( int k = 0; k < 3; k++) {
		J_temp = Simulations_[i]->get_Ji(k);
		Simulations_[i]->set_Ji(k, Simulations_[j]->get_Ji(k));
		Simulations_[j]->set_Ji(k, J_temp);
	}
	double Beta_temp = Simulations_[i]->get_S()->get_Beta();
	Simulations_[i]->get_S()->set_Beta(Simulations_[j]->get_S()->get_Beta());
	Simulations_[j]->get_S()->set_Beta(Beta_temp);

	std::swap(Simulations_[i],Simulations_[j]);
}

void ParallelTempering::Printout(std::string OutputPath) {
	std::string s = "";
	std::string OutputPath_new = "";
	for (int i = 0; i < N_simul_; i++) {
		s = std::to_string(i);
		OutputPath_new  = OutputPath + "nr_" + s;
		Simulations_[i]->Printout(OutputPath_new);
	}
	this->Printout_Energy_and_Cv(OutputPath);
	this->PrintoutMagnetisation(OutputPath);
}

void ParallelTempering::Printout_Energy_and_Cv(std::string OutputPath) const
{
	string path = OutputPath + "Energy_and_Cv.dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	double E = 0;
	double Cv = 0;
	for (int i = 0; i < N_simul_; ++i) {
		E = Simulations_[i]->first_moment_energy();
		Cv = Simulations_[i]->calculate_cv();
		*outputFileSpin << std::fixed << std::showpoint << std::setprecision(3) << beta_[i] << "\t" << E << "\t" << Cv << endl;
	}
	outputFileSpin->close();
	delete outputFileSpin;
}

void ParallelTempering::PrintoutMagnetisation(std::string OutputPath) const
{
	string path = OutputPath + "Magnetisation.dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	double M = 0;
	for (int i = 0; i < N_simul_; ++i) {
		M = Simulations_[i]->get_S()->get_magnetisation_per_spin();
		*outputFileSpin << std::fixed << std::showpoint << std::setprecision(3) << beta_[i] << "\t" << M << endl;
	}
	outputFileSpin->close();
	delete outputFileSpin;
}

void ParallelTempering::mesure_energy() {
	for(int i = 0; i < N_simul_; i++) {
		Simulations_[i]->mesure_energy();
	}
}

