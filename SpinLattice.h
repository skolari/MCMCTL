/*
 * SpinLattice.h
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */


#ifndef SPINLATTICE_H_
#define SPINLATTICE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Lattice.h"
#include "Spin.h"
#include "Random.h"

class Lattice;

class SpinLattice: public Lattice {
public:
	SpinLattice(Random* Rnd, int Deg, double J1, double J2, double J3, double Beta );
	virtual ~SpinLattice();

	void Printout(std::string outputPath) const;

	// Getters and Setters
	inline double get_Energy() { return Energy_; }
	inline double get_Beta() { return Beta_; }
	inline int get_Number_spin() { return Number_spin_; }

	inline void set_Beta(double val) {
		Beta_ = val;
	}

	inline double get_Ji(int i) {
		if ( i == 1 ) return J1_;
		if ( i == 2 ) return J2_;
		if ( i == 3 ) return J3_;
		else return 0;
	}

	inline void set_Ji(int i, double val) {
		if ( i == 1 ) J1_ = val;
		if ( i == 2 ) J2_ = val;
		if ( i == 3 ) J3_ = val;
	}

	inline double get_Spin(int i, int j) const { return S_[i][j]->getSpin(); }
	Spin* get_Spin_pointer(int i, int j) const;
	void set_Spin(Spin* S, double val);
	void set_Spin(int i, int j, double val);

	bool ifInsideLattice(int i, int j);
	std::vector<int> step_dir(int i, int j, int d);
	std::vector<int> fix_bc(int i, int j) const;

	// Energy methods
	double calculate_Energy();
	double get_energy_per_spin();
	double get_magnetisation_per_spin();
	void update_Energy();
	void update_Energy(double val);

	int number_spin();

private:
	std::vector< std::vector<Spin*> > S_;
	double Energy_;
	double J1_;
	double J2_;
	double J3_;
	double Beta_;
	int Number_spin_;
};



#endif /* SPINLATTICE_H_ */
