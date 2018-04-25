/*
 * ParallelTempering.h
 *
 *  Created on: 24.04.2018
 *      Author: Manuel Stathis
 */

#ifndef PARALLELTEMPERING_H_
#define PARALLELTEMPERING_H_

#include <vector>
#include <thread>
#include <algorithm>
#include "MonteCarlo.h"

class ParallelTempering {
public:
	ParallelTempering(int Deg, int N_simul, int N_thermal, int N_algo, int N_temp,
		double J1_start, double J2_start, double J3_start,
		double J1_end, double J2_end, double J3_end,
	);
	virtual ~ParallelTempering();

	void run();
	void algorithm_step();
	void tempering_switch(int i, int j);
	void J_swap(int i, int j);

private:
	std::vector J1_;
	std::vector J2_;
	std::vector J3_;

	int Deg_;
	int N_simul_;
	int N_thermal_;
	int N_algo_;
	int N_temp_;
	std::vector<MonteCarlo*> Simulations_;
};


#endif /* PARALLELTEMPERING_H_ */
