/*
 * LatticeTopology.h
 *
 *  Created on: 20.05.2018
 *      Author: Manuel Stathis
 */

#ifndef LATTICETOPOLOGY_H_
#define LATTICETOPOLOGY_H_

#include <vector>
#include <cmath>
#include "SpinLattice.h"
#include "Spin.h"

class LatticeTopology {
public:
	LatticeTopology();
	virtual ~LatticeTopology();

	std::vector<int> spin_neighbourhood_to_vector(Spin* S);
	int if_in_base( std::vector<int> v );
	void add_base( std::vector<int> v );
	void add_base( Spin* S );

	double calculate_geometric_mean();
	double geometric_mean_of_Lattice(SpinLattice* S);

private:
	std::vector< std::vector<int> > base_elements_;
	std::vector< double > counter_;
};


#endif /* LATTICETOPOLOGY_H_ */
