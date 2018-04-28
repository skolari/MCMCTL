/*
 * ParallelTempering.h
 *
 *  Created on: 24.04.2018
 *      Author: Manuel Stathis
 */

#ifndef PARALLELTEMPERING_H_
#define PARALLELTEMPERING_H_

#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <random>
#include <algorithm>
#include "MonteCarlo.h"

class ParallelTempering {
public:
	ParallelTempering(int Deg, int N_simul, int N_thermal, int N_algo, int N_temp,
			double J1, double J2, double J3,
			double beta_start, double beta_end);

	virtual ~ParallelTempering();

	void run();
	void algorithm_step();
	void tempering_switch(int i, int j);
	void J_swap(int i, int j);

	// Printout
	void Printout(std::string OutputPath);
	void PrintoutEnergy(std::string OutputPath) const;

private:
	int Deg_;
	int N_simul_;
	int N_thermal_;
	int N_algo_;
	int N_temp_;
	double J1_const_;
	double J2_const_;
	double J3_const_;


	std::vector<double> beta_;
	std::vector<double> J1_;
	std::vector<double> J2_;
	std::vector<double> J3_;

	std::vector<MonteCarlo*> Simulations_;

	// random
	std::random_device rd;							// Random device // @suppress("Type cannot be resolved")
	std::mt19937 mt;									// mt19937 // @suppress("Type cannot be resolved")
	std::uniform_real_distribution<double> dist;		// Uniform real distribution // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")

};


#endif /* PARALLELTEMPERING_H_ */
