#include <iostream>
#include <fstream>
#include "SpinLattice.h"
#include "Spin.h"
#include "DualLattice.h"
#include "MonteCarlo.h"
#include "ConfigFile.hpp"
#include "ConfigFile.tpp"


using namespace std;

int main(int argc, char* argv[])
{	
	std::string inputPath = "configuration.in";
	if(argc>1){
	    inputPath=argv[1];
	}

	ConfigFile configFile(inputPath);
	int Deg = configFile.get<int>("Deg");
	string outputPath = configFile.get<std::string>("outputPath");

	MonteCarlo* M = new MonteCarlo(Deg, 1000, 1000, 2);

	M->printout(outputPath);
	M->run_algorithm();
	M->printout(outputPath);

	cout << "Printout complete" << endl;
	return 0;
}
