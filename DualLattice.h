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
#include "Lattice.h"
#include "SpinLattice.h"
#include "DimerNode.h"
#include "DimerEdge.h"


class DualLattice: protected Lattice {
public:
	DualLattice(int Deg, SpinLattice* S);
	virtual ~DualLattice();

	void Printout(std::string suppl) const;
	// Getters and Setters

	// Changes the dimer value of both edges
	void switchDimer(DimerEdge* edge);
	inline DimerNode* getDimerNode(int i, int j) const {
		return Dual_[i][j];
	}
	std::vector<int> SpinDirDualNode(int i, int j, int dir) const;
protected:
	// (i, j) indices of Dimer Lattice, returns coordinates in Ajd matrix.
	inline int getDadjInd(int i, int j) const
	{
		return 2 * (N_ - 1) * j + i;
	}

	std::vector<int> fix_bc(int i, int j) const;
	std::vector< std::vector<double> > getDadj() const;

private:
	std::vector< std::vector< DimerNode* > > Dual_; //
	//std::vector<std::vector<double>> Dual_adj_; // (N, M) : N=EntrySites, M=VertexSites
	//int NDadj_;
	SpinLattice* S_;
};

#endif /* DUALLATTICE_H_ */
