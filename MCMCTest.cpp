#include <iostream>
#include <string>
#include <fstream>
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

	int Deg = 					configFile.get<int>("Deg");
	std::string outputPath = 	configFile.get<std::string>("outputPath");
	int N_simul = 				configFile.get<int>("N_simul");
	int N_thermal = 			configFile.get<int>("N_thermal");
	int N_algo = 				configFile.get<int>("N_algo");
	int N_temp = 				configFile.get<int>("N_temp");
	double J1 = 				configFile.get<double>("J1");
	double J2 = 				configFile.get<double>("J2");
	double J3 = 				configFile.get<double>("J3");
	double beta_start = configFile.get<double>("beta_start");
	double beta_end = configFile.get<double>("beta_end");

	ParallelTempering* P = new ParallelTempering(Deg, N_simul, N_thermal, N_algo, N_temp, J1, J2, J3, beta_start, beta_end);
	P->run();
	P->printout(outputPath);

	cout << "Printout complete" << endl;
	return 0;
}
