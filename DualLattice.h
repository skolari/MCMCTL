/*
 * DualLattice.h
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#ifndef DUALLATTICE_H_
#define DUALLATTICE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <tuple>

#include "Lattice.h"
#include "SpinLattice.h"
#include "DimerNode.h"
#include "DimerEdge.h"
#include "Random.h"

class DualLattice: protected Lattice {
public:
	DualLattice(Random* Rnd, int Deg, SpinLattice* S);
	virtual ~DualLattice();

	void Printout(std::string outputPath) const;
	// Getters and Setters

	// Changes the dimer value of both edges
	void switchDimer(DimerEdge* edge);

	inline DimerNode* getDimerNode(int i, int j) const {
		return Dual_[i][j];
	}

	std::vector<int> SpinDirDualNode(int i, int j, int dir) const;
	std::vector< DimerEdge* > get_local_dimer(DimerEdge* d0);
	std::vector< DimerEdge* > get_s_dimer(std::vector<DimerEdge*> d);
	std::vector< DimerEdge* > get_t_dimer(std::vector<DimerEdge*> s);
	std::tuple<std::vector< long double >, std::vector< double >> get_local_weight(DimerEdge* d0);

	inline double get_energy() { return Energy_; }
	inline void add_energy(double val) { Energy_ += val; }
	int number_spin();
	int get_vertical_dimer_index(std::vector< DimerEdge* > d);

protected:
	// (i, j) indices of Dimer Lattice, returns coordinates in Ajd matrix.
	inline int getDadjInd(int i, int j) const
	{
		return 2 * (N_ - 1) * j + i;
	}

	std::vector<int> fix_bc(int i, int j) const;
	std::vector< std::vector<double> > getDadj() const;

private:
	SpinLattice* S_;
	std::vector< std::vector< DimerNode* > > Dual_;
	double Energy_;
};

#endif /* DUALLATTICE_H_ */
