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
#include <cassert>
#include "Lattice.h"
#include "Spin.h"

class Lattice;

class SpinLattice: protected Lattice {
public:
	SpinLattice(int Deg, double J1 = 0, double J2 = 0, double J3 = 0 );
	virtual ~SpinLattice();

	void Printout(std::string suppl) const;

	// Getters and Setters
	inline double get_Energy() { return Energy_; }
	inline double get_Spin(int i, int j) const { return S_[i][j]->getSpin(); }
	inline Spin* get_Spin_pointer(int i, int j) const { return S_[i][j]; }
	void set_Spin(Spin* S, double val);
	void set_Spin(int i, int j, double val);

	bool ifInsideLattice(int i, int j);
	std::vector<int> get_direction(int i, int j, int d) const;
	std::vector<int> step_dir(int i, int j, int d);
	std::vector<int> fix_bc(int i, int j) const;
protected:
	// Energy methods
	double calculate_Energy();
	void update_Energy();
	void update_Energy(double val);
private:
	std::vector< std::vector<Spin*> > S_;	// Spin lattice S(q, r) axial coordinates
	double Energy_;
	double J1_;
	double J2_;
	double J3_;
};



#endif /* SPINLATTICE_H_ */
