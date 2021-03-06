/*
 * SpinLattice.cpp
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */

#include "SpinLattice.h"
using namespace std;


SpinLattice::SpinLattice(Random* Rnd, int Deg, bool Dipolar, double J1, double J2, double J3, double J5, double delta_J, double Beta)
	: Lattice(Rnd, Deg), S_(2 * Deg + 1, vector<Spin*>(2 * Deg + 1, NULL)), J1_(2, 0), J2_(2, 0), J3_(2, 0), J5_(2, 0), delta_J_(delta_J), Beta_(Beta),
	  a1_(2, 0),  a2_(2, 0), k1_(2, 0), k2_(2, 0)
{
	double rnd = 0;
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			S_[i][j] = new Spin(i, j);
			if (this->ifInsideLattice(i, j)) {
				rnd = Rnd_->dist();
				if (rnd < 0.5) {
					S_[i][j]->Spin::setSpin(1);
				}
				else {
					S_[i][j]->Spin::setSpin(-1);
				}
			}
		}
	}

	// Set neighbors
	vector<int> step (2, 0);
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (this->ifInsideLattice(i, j)) {
				for (int dir = 0; dir < 6; dir++) {
					step = SpinLattice::step_dir(i, j, dir);
					S_[i][j]->Spin::addNeighbor(S_[step[0]][step[1]], dir);
				}
			}
		}
	}

	if (Dipolar) {
		cout << "dipolar" << endl;
		double x = 0.5;
		double y = std::sqrt(3) / 2;
		J1_[0] = 1;
		J1_[1] = 1 / std::pow(std::sqrt(x * x + (y + delta_J) * (y + delta_J)),3);

		J2_[0] = 1 / std::pow(std::sqrt(1.5 * 1.5 + (y + delta_J) * (y + delta_J)),3);
		J2_[1] = 1 / std::pow(std::sqrt(4 * (y + delta_J) * (y + delta_J)),3);

		J3_[0] = 1/8; // 1/2
		J3_[1] = 1 / std::pow(std::sqrt(1 + 2 * (y + delta_J) * 2 * (y + delta_J)),3);

		J5_[0] = 1/std::pow(3,3);
		J5_[1] = 1 / std::pow(std::sqrt(3 * x * 3 * x + 3 * (y + delta_J) * 3 * (y + delta_J)),3);

		//cout<< J1_[0] << " " << J2_[0] << " " << J3_[0] << endl;
	} else {
		J1_[0] = J1;
		J1_[1] = J1 + delta_J;

		J2_[0] = J2;
		J2_[1] = J2;

		J3_[0] = J3;
		J3_[1] = J3;

		J5_[0] = J5;
		J5_[1] = J5;
	}

	a1_ = {1, 0};
	a2_ = {0.5, sqrt(3)* 0.5};
	double fact = 4 * M_PI / sqrt(3);
	G1_ = { 2 * M_PI, fact*(-0.5)};
	G2_= {0, fact};
	k1_ = {4 * M_PI / ( 3 * ( Deg_ + 1 )), 0};
	k2_ = {4 * M_PI / ( 3 * ( Deg_ + 1 )) * (- 0.5), 4 * M_PI / ( 3 * ( Deg_ + 1 )) * 0.5 * sqrt(3)};
	//v_0 = this->n_to_ij(0, 0);
	Normalisation_  = this->normalisation();
	Number_spin_ = this->SpinLattice::number_spin();
	Energy_ = this->calculate_Energy();

}

SpinLattice::~SpinLattice()
{
	// TODO Auto-generated constructor stub
}

void SpinLattice::Printout(string outputPath) const
{
	string path = outputPath + "_Spin.dat";

	ofstream *outputFileSpin = new ofstream();
	outputFileSpin->open(path.c_str());

	if (!outputFileSpin->is_open())
	{
		delete outputFileSpin;
		outputFileSpin = NULL;
	}

	double Spin = 0;
	vector<int> coord(2, 0);
	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			Spin = this->get_Spin(i, j);
			*outputFileSpin << i << "\t" << j << "\t" << Spin << endl;
		}
	}
	outputFileSpin->close();
	delete outputFileSpin;
}


// Getters and Setters


void SpinLattice::set_Spin(Spin* S, double val) {
	S->Spin::setSpin(val);
}

void SpinLattice::set_Spin(int i, int j, double val){
	Spin* S = this->get_Spin_pointer(i, j);
	this->set_Spin(S, val);
}


vector<int> SpinLattice::fix_bc(int i, int j) const{
	if (j == N_ - 1  && i < Nc_) {
		i = i + Deg_;
		j = 0;
	}
	if(i == N_ - 1 && j == Deg_) {
		i = Deg_;
		j = 0;
	}
	if(i == N_ - 1 && j < Nc_) {
		i = 0;
		j = j + Deg_;
	}
	if(j + i == (N_ - 1 + Deg_) && j < N_ - 1 && i < N_ - 1){
		i = i - Deg_;
		j = j - Deg_;
	}
	if (j == -1) {
		i = i - Deg_;
		j = N_ - 2;
	}
	if (i == -1) {
		i = N_ - 2;
		j = j - Deg_;
	}
	if (i + j == Deg_ - 1) {
		i = i + Deg_;
		j = j + Deg_;
	}
	vector<int> coord = {i, j};
	return coord;
}

bool SpinLattice::ifInsideLattice(int i, int j) {
	bool flag = true;
	if (i + j < Deg_) { flag = false; }
	if (i == N_ - 1) { flag = false; }
	if (j == N_ - 1) { flag = false; }
	if (i + j > N_ + Deg_ - 2) { flag = false; }
	return flag;
}

// Clockwise starting from 3 o'clock == 0
vector<int> SpinLattice::step_dir(int i, int j, int d) {
	if(!(d < 6 && d >= 0)) cerr << "dir index not in range" << endl;
	vector<int> step(2, 0);
	if (d == 0) { // before d = 1
		i = i + 1;
		step = this->fix_bc(i, j);
	} else if (d == 1){ // before d = 3
		j = j - 1;
		i = i + 1;
		step = this->fix_bc(i, j);
	} else if (d == 2) { // before d = 2
		j = j - 1;
		step = this->fix_bc(i, j);
	} else if (d == 3) { // before d = -1
		i = i - 1;
		step = this->fix_bc(i, j);
	} else if (d == 4) { // before d = -3
		j = j + 1;
		i = i - 1;
		step = this->fix_bc(i, j);
	} else if (d == 5) { // before d = -2
		j = j + 1;
		step = this->fix_bc(i, j);
	}
	return step;
}

double SpinLattice::calculate_Energy() {
	double Energy = 0;
	Spin* Spin_origin = NULL;
	Spin* Spin_neigh = NULL;
	double S_o = 0;
	double S_n = 0;
	//int dir2 = 0;

	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (this->ifInsideLattice(i, j)) {
				Spin_origin = this->SpinLattice::get_Spin_pointer(i, j);
				S_o =  Spin_origin->getSpin();
				// an other possibility is to loop over all dir and divide the final energy by 2
				// nearest neighbor
				// direction without deformation.
				Spin_neigh = Spin_origin->getNeighbor(0);
				S_n = Spin_neigh->getSpin();
				Energy = Energy + J1_[0] * S_o * S_n;

				// with deformation
				for (int dir = 1; dir < 3; dir++) {
					Spin_neigh = Spin_origin->getNeighbor(dir);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J1_[1] * S_o * S_n;
				}

				// next nearest neighbor
				//for (int dir = 0; dir < 3; dir++) {
				//dir2 = (dir + 1) % 6;
				if (J2_[0] != 0) {
					Spin_neigh = Spin_origin->getNeighbor(0)->getNeighbor(1);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J2_[0] * S_o * S_n;

					Spin_neigh = Spin_origin->getNeighbor(1)->getNeighbor(2);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J2_[1] * S_o * S_n;


					Spin_neigh = Spin_origin->getNeighbor(2)->getNeighbor(3);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J2_[0] * S_o * S_n;

				}

				//}

				// next next nearest neighbor
				if (J3_[0] != 0) {
					Spin_neigh = Spin_origin->getNeighbor(0)->getNeighbor(0);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J3_[0] * S_o * S_n;

					for (int dir = 1; dir < 3; dir++) {
						Spin_neigh = Spin_origin->getNeighbor(dir)->getNeighbor(dir);
						S_n = Spin_neigh->getSpin();
						Energy = Energy + J3_[1] * S_o * S_n;
					}
				}
				// J5
				if (J5_[0] != 0) {
					Spin_neigh = Spin_origin->getNeighbor(0)->getNeighbor(0)->getNeighbor(0);
					S_n = Spin_neigh->getSpin();
					Energy = Energy + J5_[0] * S_o * S_n;

					for (int dir = 1; dir < 3; dir++) {
						Spin_neigh = Spin_origin->getNeighbor(dir)->getNeighbor(dir)->getNeighbor(dir);
						S_n = Spin_neigh->getSpin();
						Energy = Energy + J5_[1] * S_o * S_n;
					}
				}
			}
		}
	}

	return Energy;
}

Spin* SpinLattice::get_Spin_pointer(int i, int j) const
{
	return S_[i][j];
}


void SpinLattice::update_Energy()
{
	Energy_ = this->calculate_Energy();
}

void SpinLattice::update_Energy(double val)
{
	Energy_ = Energy_ + val;
}

double SpinLattice::get_Magnetisation() {
	double M  = 0;
	for (int i = 0; i < N_; i++) {
		for (int j = 0; j < N_; j++) {
			if (this->ifInsideLattice(i, j)) {
				M += this->get_Spin(i, j);
			}
		}
	}
	return M / Number_spin_;
}

// can be optimized n = 3*deg^2
int SpinLattice::number_spin() {
	int count = 0;
	for (int i = 0; i < N_; i++) {
		for (int j = 0; j < N_; j++) {
			if (this->ifInsideLattice(i, j)) {
				count += 1;
			}
		}
	}
	return count;
}

double SpinLattice::get_energy_per_spin() {
	this->update_Energy();
	double E = this->get_Energy();
	return E / Number_spin_;
}

/*
// Fourier
vector <vector< double > > SpinLattice::correlation(int i0, int j0) {
	vector <vector< double > > corr(N_, vector<double>(N_,0));
	double S0 = this->get_Spin(i0, j0);
	for (int i = 0; i < N_; i ++) {
		for (int j = 0; j < N_; j++ ) {
			if(this->ifInsideLattice(i, j)){
				corr[i][j] = S0 * this->get_Spin(i, j);
			}
		}
	}
	return corr;
}

vector <vector< double > > SpinLattice::correlation() {
	vector <vector< double > > corr(N_, vector<double>(N_,0));
	double S0 = this->get_Spin(v_0[0], v_0[1]);
	for (int i = 0; i < N_; i ++) {
		for (int j = 0; j < N_; j++ ) {
			if(this->ifInsideLattice(i, j)){
				corr[i][j] = S0 * this->get_Spin(i, j);
			}
		}
	}
	return corr;
}

vector < int > SpinLattice::n_to_ij(int n1, int n2) {
	vector< int > v(2, 0);
	v[0] = - n2 + N_ - 2;
	v[1] = n1;
	return v;
}


double SpinLattice::fourier_transform_coeff(std::vector<double> k, vector <vector <double>> corr) {
	double coeff = 0;
	vector <int> v(2,0);
	for(int n2 = 0; n2 < Deg_; n2 ++) {
		for (int n1 = 0 - n2; n1 < Deg_; n1 ++) {
			v = this->n_to_ij(n1, n2);

			complex<double> factor (0.0, (-1) * k[0] * (n1 *a1_[0] + n2 * a2_[0]) - k[1] * (n1 *a1_[1] + n2 * a2_[1]));
			factor = exp(factor);
			coeff += corr[v[0]][v[1]] * factor.real();
		}
	}
	for(int n2 = Deg_; n2 < N_ - 1; n2 ++) {
		for (int n1 = - Deg_ + 1; n1 < N_ - n2 - 1; n1 ++) {
			v = this->n_to_ij(n1, n2);
			double im =  (-1) * k[0] * (n1 *a1_[0] + n2 * a2_[0]) -k[1] * (n1 *a1_[1] + n2 * a2_[1]);
			complex<double>factor (0.0, im);
			factor = exp(factor);
			coeff += corr[v[0]][v[1]] * factor.real();
		}
	}

	return Normalisation_ * coeff;
}

vector<vector< double >> SpinLattice::fourier_transform(std::vector < std::vector <double>> corr) {
	// call fourier coeff with k in range fo g1 g2
	// normalisation factor
	vector<vector< double >> f (N_, vector< double >(N_, 0));
	int i = 0;
	int j = 0;
	for(int n2 = -Deg_; n2 < Deg_; n2 ++) {
		j = n2 + Deg_;
		for (int n1 = -Deg_ ; n1 < Deg_; n1 ++) {
			i = n1 + Deg_;
			f[i][j] = this->fourier_transform_coeff(this->k(n1,n2), corr);
		}
	}

	return f;
}

*/
