#include "Lattice.h"

Lattice::Lattice(Random* Rnd, int Deg)
	: Rnd_(Rnd), Deg_(Deg), N_(2 * Deg + 1)
{
	// Initialization S_
	Nc_ = N_ - Deg_;
}

Lattice::~Lattice()
{

}
