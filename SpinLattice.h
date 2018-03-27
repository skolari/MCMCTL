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
#include <random>
#include <ctime>
#include <cmath>
#include "Lattice.h"

using namespace std;

class SpinLattice: protected Lattice {
public:
	SpinLattice(int Deg);
	virtual ~SpinLattice();

	virtual void Printout(string suppl) override;

	// Getters and Setters
	double get_Spin_ax(int q, int r);
	double get_Spin_cube(int x, int y, int z);

	void set_Spin_ax(int q, int r, double val);
	void set_Spin_cube(int x, int y, int z, double val);

protected:
	vector<int> Axial_to_cube(int q, int r);
	vector<int> Cube_to_axial(int x, int y, int z);

private:
	vector< vector<double> > S_;	// Spin lattice S(q, r) axial coordinates
};



#endif /* SPINLATTICE_H_ */
