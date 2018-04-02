#include <iostream>
#include <fstream>
#include <cassert>
#include "SpinLattice.h"
//#include "DualLattice.h"
using namespace std;

int main()
{	

	// SpinLattice Test;
	int Deg = 2;
	SpinLattice* S = new SpinLattice(Deg);
	bool test = false;

	// Test fix_bc
	int i = 1;
	int j = 4;
	vector<int> coord = S->fix_bc(i, j);
	test = coord[0] == 3 && coord[1] == 0;

	i = 2;
	j = 4;
	coord = S->fix_bc(i, j);
	test = test && coord[0] == 0 && coord[1] == 2;

	i = 3;
	j = 3;
	coord = S->fix_bc(i, j);
	test = test && coord[0] == 1 && coord[1] == 1;

	i = 2;
	j = 2;
	coord = S->fix_bc(i, j);
	test = test && coord[0] == 2 && coord[1] == 2;

	if(test) {
		cout << "test fix_bc passed" << endl;
	} else {
		cout << "test fix_bc failed" << endl;
	}












	// DualLattice Test
	//DualLattice* D = new DualLattice(Deg, S);
	cout << "before print out" << endl;
	S->Printout("11");
	//D->Printout("11");
	cout << "after print out" << endl;
	// delete L;
	return 0;
}
