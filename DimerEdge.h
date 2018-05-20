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
class Spin;

class DimerEdge {
public:
	DimerEdge(DimerNode* start, DimerNode* end, Spin* Spin_right, Spin* Spin_left);
	virtual ~DimerEdge();

	inline double getDimer() const
	{
		return Dimer_;
	}

	void setDimer(double val);

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
		return Spin_left_;
	}

	DimerEdge* getOppositeEdge();

	void switchDimer();

private:
	double Dimer_; // if 1 both Spin_right and Spin_left are equal, else -1.
	DimerNode* start_; // start node of the edge
	DimerNode* end_; // end node of the edge
	Spin* Spin_right_; // assiciated right spin with respect to the direction of the edge
	Spin* Spin_left_; // assiciated left spin with respect to the direction of the edge
};

#endif /* DIMEREDGE_H_ */
