#include <iostream>
#include <fstream>
#include "SpinLattice.h"
#include "DualLattice.h"
using namespace std;

int main()
{	
	int Deg = 2;
	SpinLattice* S = new SpinLattice(Deg);
	DualLattice* D = new DualLattice(Deg, S);
	cout << "before print out" << endl;
	S->Printout("11");
	D->Printout("11");
	cout << "after print out" << endl;
	// delete L;
	return 0;
}
