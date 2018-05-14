/*
 * ParallelTempering.h
 *
 *  Created on: 24.04.2018
 *      Author: Manuel Stathis
 */

#ifndef PARALLELTEMPERING_H_
#define PARALLELTEMPERING_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <omp.h>
#include <random>
#include <algorithm>
#include "MonteCarlo.h"
#include "Random.h"

class ParallelTempering {
public:
	ParallelTempering(Random* Rnd, int Deg, int N_simul, int N_thermal, int N_algo, int N_measure, int N_temp,
			double J1, double J2, double J3, double delta_J,
			double T_start, double T_end);

	virtual ~ParallelTempering();

	void run();
	void algorithm_step();
	void tempering_switch(int i, int j);
	void J_swap(int i, int j);
	void measure_energy();

	// Printout
	void Printout(std::string OutputPath);
	void Printout_Energy_and_Cv(std::string OutputPath) const;
	void PrintoutMagnetisation(std::string OutputPath) const;

private:
	Random* Rnd_;
	int Deg_;
	int N_simul_;
	int N_thermal_;
	int N_algo_;
	int N_temp_;
	int N_measure_;
	double J1_const_;
	double J2_const_;
	double J3_const_;

	std::vector<double> beta_;
	std::vector<double> J1_;
	std::vector<double> J2_;
	std::vector<double> J3_;

	std::vector<MonteCarlo*> Simulations_;
};


#endif /* PARALLELTEMPERING_H_ */
