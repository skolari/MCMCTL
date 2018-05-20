/*
 * LatticeTopology.cpp
 *
 *  Created on: 20.05.2018
 *      Author: Manuel Stathis
 */

#include "LatticeTopology.h"

using namespace std;

LatticeTopology::LatticeTopology()
: base_elements_(), counter_()
{
	// TODO Auto-generated constructor stub
}

LatticeTopology::~LatticeTopology() {
	// TODO Auto-generated destructor stub
}

std::vector<int> LatticeTopology::spin_neighbourhood_to_vector(Spin* S) {
	std::vector<int> v(6, 0);
	for(int dir = 0; dir < 6; dir ++) {
		if ( S->getNeighbor(dir)->getSpin() > 0 ) {
			v[dir] = 1;
		} else {
			v[dir] = 0;
		}
	}
	return v;
}

int LatticeTopology::if_in_base( std::vector<int> v ) {
	int flag = -1;
	int N = base_elements_.size();
	if (N == 0) {
		return flag;
	} else {
		for ( int i = 0; i < N; i++) {
			bool are_equal = true;
			for (int j = 0; j < 6; j++) {
			    if (v[j] != base_elements_[i][j])
			    {
			        are_equal = false;
			        break;
			    }
			}
			if (are_equal) {
				flag = i;
				return flag;
			}
		}
	}
	return flag;
}

void LatticeTopology::add_base( std::vector<int> v ) {
	int index = this->if_in_base(v);
	if (index >= 0) {
		counter_[index] += 1.0;
	} else {
		base_elements_.push_back(v);
		counter_.push_back(1.0);
	}
}

void LatticeTopology::add_base( Spin* S ) {
	if (S->getSpin() > 0) {
		std::vector<int> v = this->spin_neighbourhood_to_vector(S);
		this->add_base(v);
	}
}

double LatticeTopology::calculate_geometric_mean() {
	double gm = 1;
	double N = 0;
	for (auto& n : counter_) {
	    N += n;
	}
	for(unsigned i = 0; i < counter_.size(); i++ ) {
		gm = gm * (1 - counter_[i] / N);
	}
	return gm * std::exp(1);
}

double LatticeTopology::geometric_mean_of_Lattice(SpinLattice* S) {
	int N = S->get_N();
	for( int i = 0; i < N; i++ ) {
		for( int j = 0; j < N; j++ ) {
			if(S->ifInsideLattice(i, j)) {
				this->add_base( S->get_Spin_pointer(i, j));
			}
		}
	}
	double gm = this->calculate_geometric_mean();
	return gm;
}

