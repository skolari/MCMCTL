#include "Lattice.h"


Lattice::Lattice(int Deg) //TODO: periodic boundary conditions
	: Deg_(Deg), N_(2 * Deg + 1), S_(2 * Deg + 1, vector<double>(2 * Deg + 1, 0)), mt(rd()), dist(uniform_real_distribution<>(0.0, 1.0)) // @suppress("Function cannot be resolved")
{
	// init S_
	mt.seed(::time(NULL)); // @suppress("Method cannot be resolved")
	double rnd = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if ((i + j > Deg_ - 1) && (i + j < 2 * N_ - Deg_ - 1)) {
				rnd = dist(mt);
				if (rnd < 0.5) {
					S_[i][j] = 1;
				}
				else {
					S_[i][j] = -1;
				}
			}
			else {
				S_[i][j] = 0;
			}
		}
	};
}

Lattice::~Lattice()
{

}

/**

vector< vector<double> > Lattice::fromSpinToDimer(vector< vector<double> > S)
{
	vector< vector<double> > D(N_, N_);
	for(int i; i < N_; i++) {
		for(int j; j < N_; j++)  {

		}
	}
	return D;
}

vector< vector<double> > Lattice::fromDimerToSpin(vector< vector<double> > D)
{
	vector< vector<double> > S;

	return S;
}
*/


void Lattice::printoutSpin(string suppl)
{	
	string path = "./Outputfiles/Spin" + suppl + ".dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			*outputFileSpin << i << "\t" << j << "\t" << S_[i][j] << endl;
		}
	}

	outputFileSpin->close();
	delete outputFileSpin;
}
