#include "Lattice.h"
using namespace std;


Lattice::Lattice(int Deg)
	: Deg_(Deg), N_(2 * Deg + 1), mt(rd()), dist(uniform_real_distribution<>(0.0, 1.0)) // @suppress("Function cannot be resolved")
{
	// Initialization S_
	Nc_ = N_ - Deg_;
	//mt.seed(::time(NULL)); // @suppress("Method cannot be resolved")
}

Lattice::~Lattice()
{

}
