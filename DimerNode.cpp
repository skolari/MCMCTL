/*
 * DimerNode.cpp
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#include "DimerNode.h"

DimerNode::DimerNode(int i, int j)
	: Pos_(2, 0)
{
	Pos_[0] = i;
	Pos_[1] = j;
}

DimerNode::~DimerNode() {
	// TODO Auto-generated destructor stub
}

DimerEdge* DimerNode::getEdge(DimerNode* end) const {
	int l = Edges_.size();
	for(int i = 0; i < l; i++) {
		if (end == (Edges_[i]->getEnd())) {
			return Edges_[i];
		}
	}
	return NULL;
}
