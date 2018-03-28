#include "Lattice.h"
using namespace std;


Lattice::Lattice(int Deg)
	: Deg_(Deg), N_(2 * Deg + 1), mt(rd()), dist(uniform_real_distribution<>(0.0, 1.0)) // @suppress("Function cannot be resolved")
{
	// Initialization S_
	Nc_ = N_ - Deg_;
	mt.seed(::time(NULL)); // @suppress("Method cannot be resolved")
	double rnd = 0;



	//Dadj_ = Lattice::fromSpinToDimerAdj(S_);
}

Lattice::~Lattice()
{

}

/**
vector< vector<double> > Lattice::fromSpinToDimerAdj(vector< vector<double> > S)
{
	vector< vector<double> > D(NDadj_, vector<double>(NDadj_, 0.0));
	double delta = 0;
	int k = 0; // (k, l) of Dadj_
	int l = 0; // (k, l) of Dadj_
	int ii = 0; // index i of D(i,j)
	int jj = 0; // index j of D(i,j)
	vector <int> coord1(2);
	vector <int> coord2(2);
	for(int j = 0; j < N_ - 1; ++j) {
		for(int i = 0; i < N_ - 1; ++i)  {
			if (S[i][j] != 0) {
				k = Lattice::getDadjInd(2 * i, j);

				ii = 2 * i - 1;
				jj = j + 1;
				l = Lattice::getDadjInd(ii, jj);
				D[k][l] = deltaSpintoDimer(S[i][j], S[i][j + 1]);
				D[l][k] = D[k][l];

				ii = 2 * i + 1;
				jj = j - 1;
				l = Lattice::getDadjInd(ii, jj);
				D[k][l] = deltaSpintoDimer(S[i][j], S[i + 1][j]);
				D[l][k] = D[k][l];

				ii = 2 * i + 1;
				jj = j;
				l = Lattice::getDadjInd(ii, jj);
				D[k][l] = deltaSpintoDimer(Lattice::getS_ij(i + 1, j), Lattice::getS_ij(i, j + 1));
				D[l][k] = D[k][l];
			}
		}
	}
	return D;
}

/**

vector< vector<double> > Lattice::fromDimerToSpin(vector< vector<double> > D)
{
	vector< vector<double> > S;

	return S;
}
*/



/**
void Lattice::printoutDimerAdj(string suppl)
{
	string path = "./Outputfiles/DimerAdj" + suppl + ".dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	for (int i = 0; i < NDadj_; ++i) {
		for (int j = 0; j < NDadj_; ++j) {
			*outputFileSpin << Dadj_[i][j] << "\t";
		}
		*outputFileSpin << endl;
	}

	outputFileSpin->close();
	delete outputFileSpin;
}

// (i, j) indices of Dimer Lattice, returns coordinates in Ajd matrix.
int Lattice::getDadjInd(int i, int j)
{
	return N_ * j + i;
}
*/
