#include "Lattice.h"


Lattice::Lattice(int Deg) //TODO: periodic boundary conditions
	: Deg_(Deg), N_(2 * Deg + 1), S_(2 * Deg + 1, vector<double>(2 * Deg + 1, 0)), mt(rd()), dist(uniform_real_distribution<>(0.0, 1.0)) // @suppress("Function cannot be resolved")
{
	// init S_
	mt.seed(::time(NULL)); // @suppress("Method cannot be resolved")
	double rnd = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (Lattice::ifUpperBoundary(i, j) != 0){
				S_[i][j] = 0;
			}
			else if ((i + j > Deg_ - 1) && (i + j < 2 * N_ - Deg_ - 1)) {
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
	int i_max = 2 * (N_ - 1);
	int j_max = N_ - 1;
	NDadj_ = i_max * j_max;

	Dadj_ = Lattice::fromSpinToDimerAdj(S_);
}

Lattice::~Lattice()
{

}

int Lattice::ifUpperBoundary(int i, int j)
{
	if (j > 0){

		if (j == N_ - 1) {
			if(i < Deg_ + 1)
				return 1;
		}
		if (i + j == N_ + Deg_ - 1)
			return 2;

		if(i == N_ - 1){
			if(j < Deg_ + 1)
				return 3;
		}
	}
	return 0;
}

vector<int> Lattice::helpBoundaryCondition(int i, int j, int b)
{
	vector<int> coord = {i, j};
	if (b == 0){
		return coord;
	}
	else if (b == 1) {
		coord = {N_ - i - 1, 0};
		return coord;
	}
	else if (b == 2) {
		coord = {N_ - 1 - i ,N_ - 1 - j};
		return coord;
	}
	else if (b == 3) {
		coord = {0, N_ - j - 1};
		return coord;
	}
	return coord;
}

vector< vector<double> > Lattice::fromSpinToDimerAdj(vector< vector<double> > S)
{
	vector< vector<double> > D(NDadj_, vector<double>(NDadj_, 0.0));
	double delta = 0;
	int k = 0;
	int l = 0;

	vector <int> coord1(2);
	vector <int> coord2(2);
	for(int i = 0; i < N_ - 1; ++i) {
		for(int j = 0; j < N_ - 1; ++j)  {
			if (S[i][j] != 0) {

				delta = abs(S[i][j] - S[i][j + 1]);
				k = N_ * j + 2 * i - 1;
				l = N_ * j + 2 * i;
				if(delta == 2) {
					D[k][l] = 1;
					D[l][k] = D[k][l];
				} if(delta == 0){
					D[k][l] = -1;
					D[l][k] = D[k][l];
				}

				delta = abs(S[i][j] - S[i + 1][j]);
				k = N_ * (j - 1) + 2 * i + 1;
				l = N_ * j + 2 * i;
				if(delta == 2) {
					D[k][l] = 1;
					D[l][k] = D[k][l];
				} if(delta == 0){
					D[k][l] = -1;
					D[l][k] = D[k][l];
				}

				if(i + j == N_ + Deg_ - 2) {
					coord1 = Lattice::helpBoundaryCondition(i + 1, j, 2);
					coord2 = Lattice::helpBoundaryCondition(i, j + 1, 2);
					delta = abs(S_[coord1[0]][coord1[1]] - S_[coord2[0]][coord2[1]]);
					k = N_ * j + 2 * i + 1;
					l = N_ * j + 2 * i;
					if(delta == 2) {
						D[k][l] = 1;
						D[l][k] = D[k][l];
					} if(delta == 0){
						D[k][l] = -1;
						D[l][k] = D[k][l];
					}
				} else {
					delta = abs(S_[i + 1][j] - S_[i][j + 1]);
					k = N_ * j + 2 * i + 1;
					l = N_ * j + 2 * i;
					if(delta == 2) {
						D[k][l] = 1;
						D[l][k] = D[k][l];
					} if(delta == 0){
						D[k][l] = -1;
						D[l][k] = D[k][l];
					}
				}
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

	int iUB = 0;
	double Spin = 0;
	vector<int> coord(2);
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			iUB = Lattice::ifUpperBoundary(i, j);
			if (iUB == 0) {
				Spin = S_[i][j];
			} else {
				coord = Lattice::helpBoundaryCondition(i, j, iUB);
				Spin = S_[coord[0]][coord[1]];
			}
			*outputFileSpin << i << "\t" << j << "\t" << Spin << endl;
		}
	}

	outputFileSpin->close();
	delete outputFileSpin;
}

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
