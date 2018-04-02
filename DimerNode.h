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
	DimerNode();
	virtual ~DimerNode();

	inline double getPos(int i) const
	{
		assert(i < 2);
		return Pos_[i];
	}

	inline bool ifEntry() const
	{
		return ifEntry_;
	}

private:
	const std::vector<int> Pos_;
	const bool ifEntry_;
};

#endif /* DIMERNODE_H_ */
