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

class DimerEdge;

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

	inline std::vector<DimerEdge*> getEdges() const
		{
			return Edges_;
		}

	DimerEdge* getEdge(DimerNode* end) const;

	inline void addEdge(DimerEdge* edge) {
		Edges_.push_back(edge);
	}

	bool operator == (const DimerNode *other) {
	    return ((this->getPos(0) == other->getPos(0)) && (this->getPos(1) == other->getPos(1)));
	}

	bool operator != (const DimerNode *other) {
		return !((this->getPos(0) == other->getPos(0)) && (this->getPos(1) == other->getPos(1)));
	}
private:
	std::vector<int> Pos_;
	std::vector<DimerEdge*> Edges_;
};



#endif /* DIMERNODE_H_ */
