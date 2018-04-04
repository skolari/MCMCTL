/*
 * DimerEdge.cpp
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#include "DimerEdge.h"

DimerEdge::DimerEdge(DimerNode* start, DimerNode* end, Spin* Spin_right, Spin* Spin_left)
	: Spin_right_(Spin_right), Spin_left_(Spin_left)
{
	if(start->ifEntry()) {
		start_= start;
	} else {
		std::cerr << "start DimerNode has to be an entry node." << std::endl;
	}
	if(!(end->ifEntry())) {
		end_ = end;
	} else {
		std::cerr << "end DimerNode has to be an vertice node." << std::endl;
	}
	Dimer_ = std::abs(Spin_right_->getSpin() + Spin_right_->getSpin()) - 1;
}

DimerEdge::~DimerEdge() {
	// TODO Auto-generated destructor stub
}

