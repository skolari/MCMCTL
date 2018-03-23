#include <iostream>
#include <fstream>
#include "Lattice.h"

using namespace std;

int main()
{	
	int Deg = 3;
	Lattice* L = new Lattice(Deg);
	cout << "before print out" << endl;
	L->printoutSpin("1");
	L->printoutSpin("2");
	// L->printoutDimerAdj("1");
	cout << "after print out" << endl;
	// delete L;
	return 0;
}
