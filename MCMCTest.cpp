#include <iostream>
#include <string>
#include <fstream>
#include "inc\tinyxml.h"
#include "ConfigFile.hpp"
#include "ConfigFile.tpp"
#include "ParallelTempering.h"

void add_element_to_param_xml(TiXmlElement * chain, const char * in_value, double val );
void add_element_to_param_xml(TiXmlElement * chain, const char * in_value, int val );
void create_xml( int Deg, int N_simul, int N_thermal, int N_algo, int N_temp, double J1, double J2,	double J3, double beta_start, double beta_end );

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

    TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	//TiXmlElement * element1 = new TiXmlElement( "Hello1" );
	TiXmlElement * param = new TiXmlElement( "Parameter" );
	//TiXmlText * text = new TiXmlText( "World" );
	doc.LinkEndChild( decl );
	doc.LinkEndChild(param);

	//char charray[200];
	// sprintf(charray, "%2.4f", N_simul); % for double

	// Deg
	TiXmlElement* Deg_ = new TiXmlElement( "Deg" );
	std::string s = std::to_string(Deg);
	const char*  charray = s.c_str();  //use char const* as target type
	TiXmlText * text0 = new TiXmlText( charray );
	Deg_->LinkEndChild( text0 );
	param->LinkEndChild( Deg_ );

	// N_simul
	TiXmlElement* Nsimul = new TiXmlElement( "N_simul" );
	s = std::to_string(N_simul);
	charray = s.c_str();  //use char const* as target type
	TiXmlText * text1 = new TiXmlText( charray );
	Nsimul->LinkEndChild( text1 );
	param->LinkEndChild( Nsimul );

	// N_simul
	TiXmlElement* Nsimul = new TiXmlElement( "N_simul" );
	s = std::to_string(N_simul);
	charray = s.c_str();  //use char const* as target type
	TiXmlText * text1 = new TiXmlText( charray );
	Nsimul->LinkEndChild( text1 );
	param->LinkEndChild( Nsimul );

	// save
	doc.SaveFile( "madeByHand.xml" );

	// run algorithm

    //ParallelTempering* P = new ParallelTempering(Deg, N_simul, N_thermal, N_algo, N_temp, J1, J2, J3, beta_start, beta_end);
	//P->run();
	//P->Printout(outputPath);

	cout << "Printout complete" << endl;
	return 0;
}

void add_element_to_param_xml(TiXmlElement * chain, const char * in_value, int val ){
	TiXmlElement* elem = new TiXmlElement( in_value );
	std::string s = std::to_string(val);
	const char* charray = s.c_str();
	TiXmlText * text = new TiXmlText( charray );
	elem->LinkEndChild( text );
	chain->LinkEndChild( elem );
}

void add_element_to_param_xml(TiXmlElement * chain, const char * in_value, double val ){
	TiXmlElement* elem = new TiXmlElement( in_value );
	std::string s = std::to_string(val);
	const char* charray = s.c_str();
	TiXmlText * text = new TiXmlText( charray );
	elem->LinkEndChild( text );
	chain->LinkEndChild( elem );
}

void create_xml()
