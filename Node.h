/*
 * Node.h
 *
 *  Created on: 28.03.2018
 *      Author: Manuel Stathis
 */

#ifndef NODE_H_
#define NODE_H_

struct node {
	int i;
	int j;
	vector<node*> Neighbours;
	bool Entry_Site;
};

#endif /* NODE_H_ */
