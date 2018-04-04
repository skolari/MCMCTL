/*
 * DimerNode.h
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#ifndef DIMERNODE_H_
#define DIMERNODE_H_

#include <vector>
#include <cassert>
#include "DimerEdge.h"

class DimerNode {
public:

	DimerNode(int i, int j);
	virtual ~DimerNode();

	inline double getPos(int i) const
	{
		assert(i < 2);
		return Pos_[i];
	}

	inline DimerEdge* getEdge(int i) const
	{
		assert(i < Edges_.size());
		return Edges_[i];
	}

	DimerEdge* getEdge(DimerNode* end) const;

	inline void addEdge(DimerEdge* edge) {
		Edges_.push_back(edge);
	}

private:
	std::vector<int> Pos_;
	std::vector<DimerEdge*> Edges_;
};

bool operator == (DimerNode* a, DimerNode* b) {
    return ((a->getPos(0) == b->getPos(0)) && (a->getPos(1) == b->getPos(1)));
}

#endif /* DIMERNODE_H_ */
