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

	//void Printout(std::string suppl) const;
	// Getters and Setters

	//void printoutDimerAdj(string suppl);

	// Changes the dimer value of both edges
	//void changeDimer(double val);
protected:
	// (i, j) indices of Dimer Lattice, returns coordinates in Ajd matrix.
	inline int getDadjInd(int i, int j) const
	{
		return N_ * j + i;
	}

	std::vector<int> fix_bc(int i, int j) const;
	std::vector<int> SpinDirDualNode(int i, int j, int dir) const;

private:
	//vector<node*> Entry_Sites;
	std::vector< std::vector< DimerNode* > > Dual_; //
	//std::vector<std::vector<double>> Dual_adj_; // (N, M) : N=EntrySites, M=VertexSites
	//int NDadj_;
};

#endif /* DUALLATTICE_H_ */
