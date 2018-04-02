/*
 * DimerEdge.h
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#ifndef DIMEREDGE_H_
#define DIMEREDGE_H_

#include "Spin.h"
#include "DimerNode.h"

class DimerEdge {
public:
	DimerEdge();
	virtual ~DimerEdge();

	inline double getDimer() const
	{
		return *Dimer_;
	}

	inline DimerNode* getStart() const{
		return start;
	}

	inline DimerNode* getEnd() const{
		return end;
	}

	inline Spin* getSpin_right() {
		return Spin_right;
	}

	inline Spin* getSpin_left() {
		return Spin_right;
	}

private:
	double* Dimer_; // maybe add double * const
	DimerNode *start;
	DimerNode *end;
	Spin* Spin_right;
	Spin* Spin_left;
};

#endif /* DIMEREDGE_H_ */
