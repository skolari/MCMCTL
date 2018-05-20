/*
 * DimerEdge.cpp
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#include "DimerEdge.h"

DimerEdge::DimerEdge(DimerNode* start, DimerNode* end, Spin* Spin_right, Spin* Spin_left)
	: start_(start), end_(end), Spin_right_(Spin_right), Spin_left_(Spin_left)
{
	double right = Spin_right_->getSpin();
	double left = Spin_left_->getSpin();
	Dimer_ = std::abs(right + left) - 1;
}

DimerEdge::~DimerEdge() {
	// TODO Auto-generated destructor stub
}

void DimerEdge::setDimer(double val) {
	Dimer_ = val;
}

/*
 * Get the edge in the opposite direction.
 */
DimerEdge* DimerEdge::getOppositeEdge() {
	return end_->getEdge(start_);
}

/*
 * Switch the sign of the dimer value.
 */
void DimerEdge::switchDimer() {
	double dimer = (-1) * this->getDimer();
	this->setDimer(dimer);
	this->getOppositeEdge()->setDimer(dimer);
}
