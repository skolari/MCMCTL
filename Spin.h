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

	inline Spin* getNeighbor(int dir) {
		//assert(Neighbor_[dir] != NULL);
		return Neighbor_[dir];
	}

private:
	double Spin_;
	std::vector<int> Pos_; // maybe add const
	std::vector<Spin*> Neighbor_;

};

#endif /* SPIN_H_ */
