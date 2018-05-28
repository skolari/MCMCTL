/*
 * SpinLattice.h
 *
 *  Created on: 27.03.2018
 *      Author: Manuel Stathis
 */


#ifndef SPINLATTICE_H_
#define SPINLATTICE_H_
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <complex>
#include "Lattice.h"
#include "Spin.h"
#include "Random.h"

class Lattice;

class SpinLattice: public Lattice {
public:
	SpinLattice(Random* Rnd, int Deg, bool Dipolar, double J1, double J2, double J3, double J5, double delta_J, double Beta );
	virtual ~SpinLattice();

	void Printout(std::string outputPath) const;

	// Getters and Setters
	inline double get_Energy() { return Energy_; }
	inline double get_Beta() { return Beta_; }
	inline int get_Number_spin() { return Number_spin_; }

	inline void set_Beta(double val) {
		Beta_ = val;
	}

	inline std::vector< double > get_Ji(int i) {
		if ( i == 1 ) return J1_;
		if ( i == 2 ) return J2_;
		if ( i == 3 ) return J3_;
		if ( i == 5 ) return J5_;
		else {
			std::vector< double > v(2,0);
			return v;
		}
	}

	inline double get_delta_J() const {
		return delta_J_;
	}

	inline double get_Spin(int i, int j) const { return S_[i][j]->getSpin(); }
	Spin* get_Spin_pointer(int i, int j) const;
	void set_Spin(Spin* S, double val);
	void set_Spin(int i, int j, double val);

	bool ifInsideLattice(int i, int j);
	std::vector<int> step_dir(int i, int j, int d);
	std::vector<int> fix_bc(int i, int j) const;

	// Energy methods
	double calculate_Energy();
	double get_energy_per_spin();
	double get_Magnetisation();
	void update_Energy();
	void update_Energy(double val);

	int number_spin();

	// Fourier over the triangular lattice

	inline std::vector< double > r(int n1, int n2) {
		std::vector< double > r(2, 0);
		r[0] = a1_[0] * n1 + a2_[0] * n2;
		r[1] = a1_[1] * n1 + a2_[1] * n2;
		return r;
	}

	inline std::vector< double > k(int n1, int n2) {
		std::vector< double > k(2, 0);
		k[0] = k1_[0] * n1 + k2_[0] * n2;
		k[1] = k1_[1] * n1 + k2_[1] * n2;
		return k;
	}

	//std::vector <std::vector< double > > correlation(int i0, int j0);
	//std::vector <std::vector< double > > correlation();

	//std::vector < int > n_to_ij(int n1, int n2);
	//double fourier_transform_coeff(std::vector<double> k, std::vector < std::vector <double>> corr);
	//std::vector<std::vector< double >> fourier_transform(std::vector < std::vector <double>> corr);

	inline double normalisation() {
		//double V = std::pow((2 * M_PI), 2);
		double V = 1.5 * fabs(G1_[0] * G2_[1] - G1_[1] * G2_[0]);
		return 1/(V); // nicht sicher
	}
private:
	std::vector< std::vector<Spin*> > S_;
	double Energy_;
	std::vector< double >  J1_;
	std::vector< double >  J2_;
	std::vector< double >  J3_;
	std::vector< double >  J5_;

	double delta_J_;
	double Beta_;
	int Number_spin_;
	std::vector< int > winding_strings_; //winding number which is used to define n_string

	// structure vector basis
	std::vector< double > a1_;
	std::vector< double > a2_;
	std::vector< double > G1_;
	std::vector< double > G2_;
	std::vector< double > k1_;
	std::vector< double > k2_;

	//std::vector< int > v_0;
	double Normalisation_;


};



#endif /* SPINLATTICE_H_ */
