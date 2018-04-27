#include <iostream>
#include <fstream>
#include "SpinLattice.h"
#include "Spin.h"
#include "DualLattice.h"
#include "MonteCarlo.h"
#include "ConfigFile.hpp"
#include "ConfigFile.tpp"
#include "ParallelTempering.h"




int main(int argc, char* argv[])
{	
	std::string inputPath = "configuration.in";
	if(argc>1){
	    inputPath=argv[1];
	}

	ConfigFile configFile(inputPath);
	int Deg = configFile.get<int>("Deg");
	std::string outputPath = configFile.get<std::string>("outputPath");

	//MonteCarlo* M = new MonteCarlo(Deg, 1000, 1000, 2);
	ParallelTempering* P = new ParallelTempering(Deg, 5, 10, 0, 3,
			0.5, 0, 0,
			5, 0.5);
	P->printout(outputPath);
	P->run();
	P->printout(outputPath);

	cout << "Printout complete" << endl;
	return 0;
}
