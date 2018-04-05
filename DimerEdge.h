/*
 * DimerEdge.h
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#ifndef DIMEREDGE_H_
#define DIMEREDGE_H_

#include <cmath>
#include "Spin.h"
#include "DimerNode.h"

// always from entry true to false

class DimerNode;

class DimerEdge {
public:
	DimerEdge(DimerNode* start, DimerNode* end, Spin* Spin_right, Spin* Spin_left);
	virtual ~DimerEdge();

	inline double getDimer() const
	{
		return Dimer_;
	}

	inline DimerNode* getStart() const{
		return start_;
	}

	inline DimerNode* getEnd() const{
		return end_;
	}

	inline Spin* getSpin_right() {
		return Spin_right_;
	}

	inline Spin* getSpin_left() {
		return Spin_right_;
	}

private:
	double Dimer_; // if 1 both Spin_right and Spin_left are equal, else -1.
	DimerNode* start_;
	DimerNode* end_;
	Spin* Spin_right_;
	Spin* Spin_left_;
};

#endif /* DIMEREDGE_H_ */
