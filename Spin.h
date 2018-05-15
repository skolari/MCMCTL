/*
 * Spin.h
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#ifndef SPIN_H_
#define SPIN_H_

#include <vector>
#include <iostream>
#include "DimerEdge.h"

class DimerEdge;

class Spin {
public:
	Spin(int i, int j, double Spin = 0);
	virtual ~Spin();

	void setSpin(double newSpin);
	inline double getSpin() const {
		return Spin_;
	}
	inline double getPos(int i) const
	{
		if(!(i < 2 && i >= 0)) std::cerr << "pos not in range" << std::endl;
		return Pos_[i];
	}

	void addNeighbor(Spin* newNeigh, int dir);
	void setDimer(DimerEdge* newDimer, int dir);

	inline Spin* getNeighbor(int dir) {
		//assert(Neighbor_[dir] != NULL);
		return Neighbor_[dir];
	}

	inline int getDirNeighbor(Spin* Neighbor){
		int dir = 0;
		while(Neighbor_[dir] != Neighbor && dir < 6) {
			dir += 1;
		}
		return dir;
	}

	inline DimerEdge* getDimer(int dir) {
		return Dimer_[dir];
	}

	bool operator == (const Spin *other) {
	    return ((this->getPos(0) == other->getPos(0)) && (this->getPos(1) == other->getPos(1)));
	}

	bool operator != (const Spin *other) {
		return !((this->getPos(0) == other->getPos(0)) && (this->getPos(1) == other->getPos(1)));
	}

private:
	double Spin_;
	std::vector<int> Pos_; // maybe add const
	std::vector<Spin*> Neighbor_;
	std::vector<DimerEdge*> Dimer_;
};

#endif /* SPIN_H_ */
