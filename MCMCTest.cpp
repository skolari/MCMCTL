#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include "inc/tinyxml.h"
#include "ConfigFile.hpp"
#include "ConfigFile.tpp"
#include "ParallelTempering.h"
#include "Random.h"

void add_element_to_xml(TiXmlElement * chain, const char * in_value, double val );
void add_element_to_xml(TiXmlElement * chain, const char * in_value, int val );
void add_element_to_xml(TiXmlElement * chain, const char * in_value, std::string s );
void add_element_to_xml(TiXmlElement * chain, const char * in_value, char* c );
void create_xml(std::string inputPath, char* time_normal, std::string time_sec);

std::string time_t_to_string(time_t t);

int main(int argc, char* argv[])
{	
	// get time
    std::time_t result = std::time(nullptr);
	char* time_normal = std::asctime(std::localtime(&result));
	std::string time_sec = time_t_to_string(result);



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
	int N_mesure =				configFile.get<int>("N_mesure");
	double J1 = 				configFile.get<double>("J1");
	double J2 = 				configFile.get<double>("J2");
	double J3 = 				configFile.get<double>("J3");
	double beta_start = 		configFile.get<double>("beta_start");
	double beta_end = 			configFile.get<double>("beta_end");

	// create xml
	create_xml(inputPath, time_normal, time_sec);

	// run algorithm
	int N = 2 * Deg + 1;
	Random* Rnd = new Random(N);
    ParallelTempering* P = new ParallelTempering(Rnd, Deg, N_simul, N_thermal, N_algo, N_temp, N_mesure, J1, J2, J3, beta_start, beta_end);
	P->run();
	P->Printout(outputPath);

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

void add_element_to_xml(TiXmlElement * chain, const char * in_value, char* c ){
	TiXmlElement* elem = new TiXmlElement( in_value );
	TiXmlText * text = new TiXmlText( c );
	elem->LinkEndChild( text );
	chain->LinkEndChild( elem );
}

void create_xml(std::string inputPath, char* time_normal, std::string time_sec) {
	ConfigFile configFile(inputPath);

	int Deg = 					configFile.get<int>("Deg");
	std::string outputPath = 	configFile.get<std::string>("outputPath");
	int N_simul = 				configFile.get<int>("N_simul");
	int N_thermal = 			configFile.get<int>("N_thermal");
	int N_algo = 				configFile.get<int>("N_algo");
	int N_temp = 				configFile.get<int>("N_temp");
	int N_mesure =				configFile.get<int>("N_mesure");
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
	add_element_to_xml(general, "Time", time_normal);
	add_element_to_xml(general, "Timestamp", time_sec);

	// param
	add_element_to_xml(param, "Deg", Deg);
	add_element_to_xml(param, "N_simul", N_simul);
	add_element_to_xml(param, "N_thermal", N_thermal);
	add_element_to_xml(param, "N_algo", N_algo);
	add_element_to_xml(param, "N_temp", N_temp);
	add_element_to_xml(param, "N_mesure", N_mesure);
	add_element_to_xml(param, "J1", J1);
	add_element_to_xml(param, "J2", J2);
	add_element_to_xml(param, "J3", J3);
	add_element_to_xml(param, "Beta_start", beta_start);
	add_element_to_xml(param, "Beta_end", beta_end);

	// save
	outputPath = outputPath + time_sec +".xml";
	const char* charray = outputPath.c_str();
	doc.SaveFile( charray );
}

std::string time_t_to_string(time_t t)
{
    std::stringstream sstr;
    sstr << t;
    return sstr.str();
}
