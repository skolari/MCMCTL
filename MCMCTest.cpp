#include <iostream>
#include <fstream>
#include "SpinLattice.h"
#include "Spin.h"
#include "DualLattice.h"
#include "MonteCarlo.h"
using namespace std;

int main()
{	
	int Deg = 5;
	MonteCarlo* M = new MonteCarlo(Deg, 1, 0, 0);
	M->printout("1");
	M->run_algorithm();
	M->printout("2");
	cout << "Printout complete" << endl;
	/*
	// SpinLattice Test;
	int Deg = 5;
	SpinLattice* S = new SpinLattice(Deg);
	bool test = false;

	// Test fix_bc deg = 2
	if (Deg == 2){
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

		i = 1;
		j = 0;
		coord = S->fix_bc(i, j);
		test = test && coord[0] == 3 && coord[1] == 2;

		i = 4;
		j = 2;
		coord = S->fix_bc(i, j);
		test = test && coord[0] == 0 && coord[1] == 4;

		if(test) {
			cout << "YES! test fix_bc passed" << endl;
		} else {
			cout << "NO! :( test fix_bc failed" << endl;
		}


		// Test step_dir
		i = 3;
		j = 2;
		int dir = 0;
		coord = S->step_dir(i, j, dir);
		test = coord[0] == 0 && coord[1] == 4;

		dir = 1;
		coord = S->step_dir(i, j, dir);
		test = test && coord[0] == 0 && coord[1] == 3;

		dir = 3;
		coord = S->step_dir(i, j, dir);
		test = test && coord[0] == 2 && coord[1] == 2;

		dir = 5;
		coord = S->step_dir(i, j, dir);
		test = test && coord[0] == 1 && coord[1] == 1;

		if(test) {
			cout << "YES! test step_dir passed" << endl;
		} else {
			cout << "NO!:( test step_dir failed" << endl;
		}
	}


	// DualLattice Test
	DualLattice* D = new DualLattice(Deg, S);
	cout << "before print out" << endl;
	S->Printout("11");
	D->Printout("11");
	cout << "after print out" << endl;
	// delete L;
	 *
	 */
	return 0;
}
