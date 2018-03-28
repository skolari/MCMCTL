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


class SpinLattice: protected Lattice {
public:
	SpinLattice(int Deg);
	virtual ~SpinLattice();

	void Printout(std::string suppl) const;

	// Getters and Setters
	double get_Spin_ax(int q, int r) const;
	double get_Spin_cube_periodic(int x, int y, int z) const;

	void set_Spin_ax(int q, int r, double val);
	void set_Spin_cube(int x, int y, int z, double val);

protected:
	std::vector<int> Axial_to_cube(int q, int r) const;
	std::vector<int> Cube_to_axial(int x, int y, int z) const;
	int make_cube_periodic(int x) const;
	bool Cube_same_position(int x1, int x2, int x3, int y1, int y2, int y3);
private:
	std::vector< std::vector<double*> > S_;	// Spin lattice S(q, r) axial coordinates
};



#endif /* SPINLATTICE_H_ */
