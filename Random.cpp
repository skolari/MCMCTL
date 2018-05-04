/*
 * Random.cpp
 *
 *  Created on: 04.05.2018
 *      Author: Manuel Stathis
 */

#include "Random.h"

Random::Random(int N)
	: 	N_(N),
		mt(rd()),
		dist_(std::uniform_real_distribution<>(0.0, 1.0)),
		dist_2_(std::uniform_int_distribution<>(0, 1)),
		dist_3_(std::uniform_int_distribution<>(0, 2)),
		dist_dual_n_vertical_(std::uniform_int_distribution<>(0, N_ -  2)),
		dist_dual_n_horizontal_(std::uniform_int_distribution<>(0, 2 * (N_ - 1) - 1))
{
	mt.seed(::time(NULL));
}

Random::~Random() {
	// TODO Auto-generated destructor stub
}
