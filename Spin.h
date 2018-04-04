/*
 * Spin.h
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#ifndef SPIN_H_
#define SPIN_H_

#include <vector>
#include <cassert>
#include <iostream>

// struct Neighbor;

class Spin {
public:
	Spin(int i, int j, double Spin = 0);
	virtual ~Spin();

	inline double getSpin() const
	{
		return Spin_;
	}

	void setSpin(double newSpin);

	inline double getPos(int i) const
	{
		assert(i < 2 && i >= 0);
		return Pos_[i];
	}

	void addNeighbor(Spin* newNeigh, int dir);

	inline Spin* getNeighbor(int dir) {
		return Neighbor_[dir];
	}

private:
	double Spin_;
	std::vector<int> Pos_; // maybe add const
	std::vector<Spin*> Neighbor_;

};
/*
bool operator == (Spin* a, Spin* b) {
     ((a->getPos(0) == b->getPos(0)) && (a->getPos(1) == b->getPos(1)));
}

bool operator != (Spin* a, Spin* b) {
    return !(a == b);
}
*/
/*
struct Neighbor {
	Spin* Neighbor;
	double* Dimer;
};
*/
#endif /* SPIN_H_ */
