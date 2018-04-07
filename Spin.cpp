/*
 * Spin.cpp
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#include "Spin.h"

Spin::Spin(int i, int j, double Spin)
	: Spin_(Spin), Pos_(2, 0), Neighbor_(6, NULL)
{
	Pos_[0] = i;
	Pos_[1] = j;
}

Spin::~Spin() {
	// TODO Auto-generated destructor stub
}

void Spin::setSpin(double newSpin)
{
	Spin_ = newSpin;
}

// direction dir goes from 0 to 5
void Spin::addNeighbor(Spin* newNeigh, int dir){
	if(newNeigh == NULL) std::cerr << "newNeigh must not be empty" << std::endl;
	Neighbor_[dir] = newNeigh;
}


