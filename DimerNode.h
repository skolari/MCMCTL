/*
 * DimerNode.h
 *
 *  Created on: 01.04.2018
 *      Author: Manuel Stathis
 */

#ifndef DIMERNODE_H_
#define DIMERNODE_H_

#include <vector>
#include "DimerEdge.h"

class DimerEdge;

class DimerNode {
public:

	DimerNode(int i, int j);
	virtual ~DimerNode();

	inline double getPos(int i) const
	{
		if (!(i < 2)) std::cerr << "pos index not in rage" << std::endl;
		return Pos_[i];
	}

	inline DimerEdge* getEdge(int i) const
	{
		return Edges_[i];
	}

	inline std::vector<DimerEdge*> getEdges() const
	{
		if(!(Edges_.empty())) {
			return Edges_;
		} else {
			return {};
		}
	}

	inline int numberEdges() const {
		return Edges_.size();
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

	inline void printoutPos() {
		std::cout << "i = " << this->getPos(0) << ", j = " << this->getPos(1) << std::endl;
	}
private:
	std::vector<int> Pos_;
	std::vector<DimerEdge*> Edges_;
};



#endif /* DIMERNODE_H_ */
