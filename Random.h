/*
 * Random.h
 *
 *  Created on: 04.05.2018
 *      Author: Manuel Stathis
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>
#include <ctime>

class Random {
public:
	Random( int N );
	virtual ~Random();

	inline double dist() {
		return dist_(mt);
	}

	inline int dist_2() {
		return dist_2_(mt);
	}

	inline int dist_3() {
		return dist_3_(mt);
	}

	inline int dist_dual_n_horizontal() {
		return dist_dual_n_horizontal_(mt);
	}

	inline int dist_dual_n_vertical() {
		return dist_dual_n_vertical_(mt);
	}
	inline int piecewise_constant_distribution(std::piecewise_constant_distribution<> dist) {
		int integer = (int) dist(mt);
		return integer;
	}
private:
	int N_;
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_real_distribution<double> dist_; // Uniform real distribution
	std::uniform_int_distribution<int> dist_2_; // chooses randomly 0 to 1
	std::uniform_int_distribution<int> dist_3_; // chooses randomly 0 to 2
	std::uniform_int_distribution<int> dist_dual_n_vertical_; // chooses randomly 0 to N-1, N size of spin lattice.
	std::uniform_int_distribution<int> dist_dual_n_horizontal_; // chooses randomly 0 to N-1, N size of spin lattice.

};

#endif /* RANDOM_H_ */
