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


class DualLattice: protected Lattice {
public:
	DualLattice(int Deg, SpinLattice* S);
	virtual ~DualLattice();

	std::vector<std::vector<double>> From_Spin_to_Dual(SpinLattice* S);

	void Printout(std::string suppl) const;
	// Getters and Setters
	//void set_dimer(node n1, node n2, double val);
	//double get_dimer(node n1, node n2);
	//void printoutDimerAdj(string suppl);

protected:
	inline int getDadjInd(int i, int j) const;
private:
	//vector<node*> Entry_Sites;
	//vector<vector<node*>> Dual_; //
	std::vector<std::vector<double>> Dual_adj_; // (N, M) : N=EntrySites, M=VertexSites
	int NDadj_;
};

#endif /* DUALLATTICE_H_ */
