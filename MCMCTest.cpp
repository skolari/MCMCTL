#include <iostream>
#include <string>
#include <fstream>
#include "inc\tinyxml.h"
#include "ConfigFile.hpp"
#include "ConfigFile.tpp"
#include "ParallelTempering.h"

void add_element_to_xml(TiXmlElement * chain, const char * in_value, double val );
void add_element_to_xml(TiXmlElement * chain, const char * in_value, int val );
void create_xml(std::string inputPath);

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
	double beta_start = 		configFile.get<double>("beta_start");
	double beta_end = 			configFile.get<double>("beta_end");

	// create xml
	create_xml(inputPath);
	// run algorithm

    //ParallelTempering* P = new ParallelTempering(Deg, N_simul, N_thermal, N_algo, N_temp, J1, J2, J3, beta_start, beta_end);
	//P->run();
	//P->Printout(outputPath);

	cout << "Printout complete" << endl;
	return 0;
}

void add_element_to_xml(TiXmlElement * chain, const char * in_value, int val ){
	TiXmlElement* elem = new TiXmlElement( in_value );
	std::string s = std::to_string(val);
	const char* charray = s.c_str();
	TiXmlText * text = new TiXmlText( charray );
	elem->LinkEndChild( text );
	chain->LinkEndChild( elem );
}

void add_element_to_xml(TiXmlElement * chain, const char * in_value, double val ){
	TiXmlElement* elem = new TiXmlElement( in_value );
	std::string s = std::to_string(val);
	const char* charray = s.c_str();
	TiXmlText * text = new TiXmlText( charray );
	elem->LinkEndChild( text );
	chain->LinkEndChild( elem );
}

void add_element_to_xml(TiXmlElement * chain, const char * in_value, std::string s ){
	TiXmlElement* elem = new TiXmlElement( in_value );
	const char* charray = s.c_str();
	TiXmlText * text = new TiXmlText( charray );
	elem->LinkEndChild( text );
	chain->LinkEndChild( elem );
}

void create_xml(std::string inputPath) {
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
	double beta_start = 		configFile.get<double>("beta_start");
	double beta_end = 			configFile.get<double>("beta_end");

	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	TiXmlElement * general = new TiXmlElement( "General" );
	TiXmlElement * param = new TiXmlElement( "Simulation_Parameters" );
	doc.LinkEndChild( decl );
	doc.LinkEndChild( general );
	doc.LinkEndChild( param );

	// general
	//git
	//time

	// param
	add_element_to_xml(param, "Deg", Deg);
	add_element_to_xml(param, "N_simul", N_simul);
	add_element_to_xml(param, "N_thermal", N_thermal);
	add_element_to_xml(param, "N_algo", N_algo);
	add_element_to_xml(param, "N_temp", N_temp);
	add_element_to_xml(param, "J1", J1);
	add_element_to_xml(param, "J2", J2);
	add_element_to_xml(param, "J3", J3);
	add_element_to_xml(param, "Beta_start", beta_start);
	add_element_to_xml(param, "Beta_end", beta_end);

	// save
	doc.SaveFile( "madeByHand.xml" );

}
