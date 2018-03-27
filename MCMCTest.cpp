#include <iostream>
#include <fstream>
#include "SpinLattice.h"

using namespace std;

int main()
{	
	int Deg = 3;
	SpinLattice* L = new SpinLattice(Deg);
	cout << "before print out" << endl;
	L->Printout("11");
	//L->printoutDimerAdj("1");
	cout << "after print out" << endl;
	// delete L;
	return 0;
}
