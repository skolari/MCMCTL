/*
 * MonteCarlo.cpp
 *
 *  Created on: 06.04.2018
 *      Author: Manuel Stathis
 */

#include "MonteCarlo.h"
using namespace std;

MonteCarlo::MonteCarlo(Random* Rnd, int Deg, int N_thermal, int N_algo, bool Dipolar, double J1, double J2, double J3, double J5, double delta_J, double Beta)
	: Rnd_(Rnd), worm_(), winding_strings_(2,0), energy_measures_(), magnetisation_measures_(), nstring_measures_(), N_thermal_(N_thermal), N_algo_(N_algo)
{
	S_ = new SpinLattice(Rnd, Deg, Dipolar, J1, J2, J3, J5, delta_J, Beta);
	D_ = new DualLattice(Rnd, Deg, S_);
	entry_node_ = NULL;
	winding_number_2 = 0;
	winding_number_1 = 0;

	Deg_ = S_->Lattice::get_Deg();
	N_ = S_->Lattice::get_N();
}

MonteCarlo::~MonteCarlo() {
	// TODO Auto-generated destructor stub
}

/**
 * Initialize the algorithm algorithm.
 */
void MonteCarlo::init_update() {

	worm_.clear();

	int rnd_i = 0;
	int rnd_j = 0;
	int rnd = 0;

	do {
		rnd_i = Rnd_->dist_dual_n_horizontal();
		rnd_j = Rnd_->dist_dual_n_vertical();
	} while(D_->getDimerNode(rnd_i, rnd_j)->numberEdges() == 0);

	entry_node_ = D_->getDimerNode(rnd_i, rnd_j);
	rnd = Rnd_->dist_3();
	worm_.push_back(entry_node_->getEdge(rnd));

	this->update_winding_number();
	this->proba_step();
}

// maybe there is a more efficient way.
void MonteCarlo::myopic_step() {
	DimerEdge* last_edge = worm_.back();
	vector<DimerEdge*> d = D_->get_local_dimer(last_edge);

	DimerNode* end_node = last_edge->getEnd();
	DimerNode* start_node = last_edge->getStart();
	DimerNode* new_end = NULL;
	DimerEdge* next_edge = NULL;
	double rnd = 0;

	do{ // maybe create function, edges_without(dimerEdge*)
		rnd = Rnd_->dist_3();
		next_edge = end_node->getEdge(rnd);
		new_end = next_edge->getEnd();
	} while(new_end == start_node);

	worm_.push_back(next_edge);
	this->update_winding_number();

}

/*
 * the probabilistic step of the algorihm.
 */
void MonteCarlo::proba_step() {

	DimerEdge* d0 = worm_.back();
	DimerEdge* next_edge = NULL;

	std::vector< DimerEdge* > d = D_->get_local_dimer(d0);

	tuple<vector< long double >, vector< double >> foo = D_->get_local_weight(d0);
	vector< long double > W;
	vector< double > delta_E;
	std::tie (W, delta_E) = foo;
	//cout << delta_E[0] << " , "  << delta_E[1] << " , " <<delta_E[2] << endl;

	std::vector< std::vector<long double>> M = this->get_M(W);
	std::vector<double> i{0, 1, 2, 3};
	//std::vector<long double> w{M[0][0], M[0][1], M[0][2]};


	//std::piecewise_constant_distribution<double> dist(i.begin(), i.end(), w.begin());

	//int next_index = int(Rnd_->piecewise_constant_distribution(dist));
	double rnd = Rnd_->dist();
	//cout << "rnd. " << rnd << endl;
	int next_index = 4;
	if (rnd < M[0][0]) {
		next_index = 0;
	}
	else if ((rnd >= M[0][0]) && (rnd < M[0][0] + M[0][1])) {
		next_index = 1;
	}
	else {
		next_index = 2;
	}
	//cout << next_index << endl;

	if (next_index == 0) {
		next_edge = d[0]->getOppositeEdge();
	}

	else if (next_index == 1) {
		D_->switchDimer(d[0]);
		D_->switchDimer(d[1]);
		next_edge = d[1];
		//S_->update_Energy(delta_E[1]);
	}

	else if (next_index == 2) {
		D_->switchDimer(d[0]);
		D_->switchDimer(d[2]);
		next_edge = d[2];
		//S_->update_Energy(delta_E[0]);
	}

	//cout << "next index: "<<next_index << endl;
	worm_.push_back(next_edge);

	this->update_winding_number();

	this->get_S()->update_Energy();
}

/*
 * create one update
 * 1. initialize the algorithm
 * 2. do one myopic step
 * 3. do one probabilistic step
 * 4. repeat till worm is closed
 * 5. check winding number and accept if even else restart.
 */
void MonteCarlo::create_update() {
	this->init_update();
	DimerEdge* last_edge = worm_.back();
	DimerNode* end_node = last_edge->getEnd();
	int count = 0;
	int count_max = 20 * S_->get_Number_spin();

	do {
		this->myopic_step();
		this->proba_step();
		last_edge = worm_.back();
		end_node = last_edge->getEnd();
		count += 1;
	} while(end_node != entry_node_ && count < count_max);


	if(count_max == count ||count_max < winding_number_2 || count_max < winding_number_1 ) {
		this->delete_worm();
		winding_number_2 = 0;
		winding_number_1 = 0;
		//cerr << "Too long worm." << endl;
		this->create_update();
	}

	//cout << winding_number_1 << " and " << winding_number_2 << endl;
	if ((winding_number_2 % 2 == 0)
			&& (winding_number_1 % 2 == 0)) {
		this->map_dimer_to_spin();

		winding_number_2 = 0;
		winding_number_1 = 0;

		worm_.clear();
	}
	else {
		this->create_update();
	}
}

/*
 * runs the simulation without parallel tempering and measures the energy every 20 steps during the N_algo_ loop.
 */
void MonteCarlo::run_algorithm_single() {
	// N_thermal_ loop, thermalisation
	for ( int i = 0; i < N_thermal_; i++ ) {
		this->create_update();
	}

	// N_algo_ loop
	for ( int i = 0; i < N_algo_; i++ ) {
		this->create_update();
		if (i % 20 == 0){
			this->measure();
		}
	}
}

/*
 * creates N_temp updates, used during parallel tempering.
 * @param N_temp number uf updates
 */
void MonteCarlo::run_parallel_step(int N_temp) {
	for ( int i = 0; i < N_temp; i++ ) {
		this->create_update();
	}
}

void MonteCarlo::Printout(std::string OutputPath){
	S_->Printout(OutputPath);
	D_->Printout(OutputPath);
}

std::vector< std::vector<long double>> MonteCarlo::get_M(std::vector <long double> W)
{
	std::vector< std::vector<long double>> M(3, std::vector<long double>(3, 0));
	std::vector< std::vector<long double>> A(3, std::vector<long double>(3, 0));
	long double W_max = *std::max_element(W.begin(), W.end()); // max of W
	int i_max = std::distance(std::begin(W), std::max_element(W.begin(), W.end())); // index of max element

	long double W_other = 0;
	vector<int> m(2,0);
	int j = 0;
	//cout << "W0: " << W[0] << ", W1: " << W[1] << ", W2: " << W[2] << endl;
	for (int i = 0; i < 3; i ++) {
		if (i != i_max){
			W_other += W[i];
			m[j] = i;
			j += 1;
		}
	}

	if (W_max > W_other) {
		//cout << "bigger" << endl;
		//cout << "i_max[: " << i_max << endl;
		//cout << "m[0]: " << m[0] << ", m1: " << m[1]<<endl;
		A[i_max][i_max] = W[i_max] - W[m[0]] - W[m[1]];

		A[m[0]][i_max] = W[m[0]];
		A[i_max][m[0]] = A[m[0]][i_max];

		A[m[1]][i_max] = W[m[1]];
		A[i_max][m[1]] = A[m[1]][i_max];
	} else {
		//cout << "smaller" << endl;
		A[0][1] = 0.5 * (W[0] + (W[1] - W[2]));
		A[1][0] = A[0][1];

		A[0][2] = 0.5 * ( W[0] + (W[2] - W[1]));
		A[2][0] = A[0][2];

		A[1][2] = 0.5 * ( W[1] + (W[2] - W[0]));
		A[2][1] = A[1][2];
	}

	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			M[i][j] = A[i][j] / W[i];
		}
	}
	//std::cout << "M_0: " << M[0][0] << ", M_1: " << M[0][1] << ", M_2: " << M[0][2] << ", sum: " << M[0][0]+ M[0][1]+ M[0][2] << endl;

	if (std::fabs(M[0][0]+ M[0][1]+ M[0][2] - 1) > 1e-10) {
		cout << "sum of probabilities is not 1!!" << endl;
		std::cout << "M_0: " << M[0][0] << ", M_1: " << M[0][1] << ", M_2: " << M[0][2] << ", sum: " << M[0][0]+ M[0][1]+ M[0][2] << endl;
		cout << "W0: " << W[0] << ", W1: " << W[1] << ", W2: " << W[2] << endl;
	}

	return M;
}

/*
 * This maps a dimer configuration of the dual lattice to a spin configuration of the spin lattice.
 */
void MonteCarlo::map_dimer_to_spin() {
	// update vertical Spins
	int i = 0;
	for (int j = 0; j < Deg_; j++) {
		i = Deg_- j;
		if(S_->ifInsideLattice(i, j)) {
			this->update_spin_neighbor_dir(i, j, 4);
		}
	}
	for (int j = Deg_; j < N_ - 2; j++) {
		i = 0;
		this->update_spin_neighbor_dir(i, j, 5);
	}

	// update horizontal Spins
	for (int j = Deg_; j >= 0; j--) {
		for (int i = Deg_ - j; i < N_ - 2; i++){
			if(S_->ifInsideLattice(i, j)) {
				this->update_spin_neighbor_dir(i, j, 0);
			}
		}
	}

	for (int j = Deg_ + 1; j < N_ - 1; j++) {
		for (int i = 0; i < N_ + Deg_ - j - 2; i++){
			if(S_->ifInsideLattice(i, j)) {
				this->update_spin_neighbor_dir(i, j, 0);
			}
		}
	}

}

/*
 * Changes the spin value of a neighbor spin in a certain direction in function of the dimer between them.
 * @param i i-index of origin spin
 * @param j j-index of origin spin
 * @param dir direction of neighbor which will be updated
 */
void MonteCarlo::update_spin_neighbor_dir(int i, int j, int dir) {
	double val = 0;
	DimerEdge* dimer = S_->get_Spin_pointer(i, j)->getDimer(dir);
	double d = dimer->getDimer();
	val = d * S_->get_Spin(i, j);
	dimer->getSpin_left()->setSpin(val);
}

/*
 * adds a new measure to the energy_measures_ vector
 */
void MonteCarlo::measure() {
	//double E = S_->get_energy_per_spin();
	S_->update_Energy();
	double E = S_->get_Energy();
	double M = S_->get_Magnetisation();
	energy_measures_.push_back(E);
	if (E != -48) {
		this->Printout("./Debugg/fuck");
		char c;
		cin >> c;
	}
	magnetisation_measures_.push_back(M);

	//n_strings
	double N_3 = S_->get_Number_spin() / 3.0;
	double L = std::sqrt(N_3);
	this->calculate_winding_strings();
	double n_string = 2/3 - (winding_strings_[0] + winding_strings_[1])/(3 * L);
	nstring_measures_.push_back(n_string);
}

/*
 * the first moment of the energy over all measurements (till now) of this simulation
 * @return first moment of the energy
 */
double MonteCarlo::first_moment_energy() {
	int Number_sites = S_->get_Number_spin();
	double energy_sum = 0;
	int N = energy_measures_.size();
	for (int i = 0; i < N; i++) {
	    energy_sum += energy_measures_[i];
	}
	energy_sum  = energy_sum / (N * Number_sites);
	return energy_sum;
}

/*
 * the first moment of the vector
 * @return second moment of a vector
 */
double MonteCarlo::first_moment(vector< double > v) {
	double sum = 0;
	int N = v.size();
	for (int i = 0; i < N; i++) {
	    sum = sum + v[i];
	}
	sum  = sum / N;
	return sum;
}

/*
 * the second moment of the vector
 * @return second moment of a vector
 */
double MonteCarlo::second_moment(vector< double > v) {
	double sum = 0;
	int N = v.size();
	for (int i = 0; i < N; i++) {
	    sum = sum + v[i] * v[i];
	}
	sum  = sum / N;
	return sum;
}

/*
 * the forth moment of the vector
 * @return forth moment of a vector
 */
double MonteCarlo::forth_moment(vector< double > v) {
	double sum = 0;
	int N = v.size();
	for (int i = 0; i < N; i++) {
	    sum = sum + v[i] * v[i] * v[i] * v[i];
	}
	sum  = sum / N;
	return sum;
}

double MonteCarlo::variance_energy() {
	int Number_sites = S_->get_Number_spin();
	double energy_first = this->first_moment_energy();
	double energy_second = this->second_moment(energy_measures_) / Number_sites;
	return energy_second - energy_first * energy_first;
}

/*
 * Calculates the heat capacity (Cv) calculated wtih the first and second moment of the energy.
 * @return Cv
 */
double MonteCarlo::calculate_cv() {
	int Number_sites = S_->get_Number_spin();
	double beta = S_->get_Beta();
	double first_moment_energy  = this->first_moment_energy();
	double second_moment_energy = this->second_moment(energy_measures_) / Number_sites;
	double cv =  (second_moment_energy - Number_sites * first_moment_energy * first_moment_energy) * beta * beta;
	return cv;
}

/*
 * Calculates the Binder Cumulant .
 * @return B_c
 */
double MonteCarlo::calculate_binder_cumulant() {
	double second_moment_mag  = this->second_moment(magnetisation_measures_);
	double forth_moment_energy = this->forth_moment(magnetisation_measures_);
	double Bc = 1 - forth_moment_energy / ( 3 * second_moment_mag * second_moment_mag );
	return Bc;
}

double MonteCarlo::first_moment_nstring() {
	double nstring = this->first_moment(nstring_measures_);
	return nstring;
}

void MonteCarlo::delete_worm() {
	int N = worm_.size();
	for (int i = 0; i < N; i++ ) {
		worm_[i]->switchDimer();
	}
	worm_.clear();
}


/*
 * Updates the winding number in function the last edge of the worm_.
 */
void MonteCarlo::update_winding_number() {
	DimerEdge* last_edge = worm_.back();

	int j_start = last_edge->getStart()->getPos(1);
	int j_end = last_edge->getEnd()->getPos(1);
	int delta_j = std::abs(j_start - j_end);
	int i_start = last_edge->getStart()->getPos(0);
	int i_end = last_edge->getEnd()->getPos(0);
	int delta_i = std::abs(i_start - i_end);

	// Winding number of horizontal lines lines update

	if (j_end == N_ - 2 && i_end != N_ - 1) {
		if (j_start == N_ - 3) {
			winding_number_2 += 1;
		}
	}
	if (j_start == N_ - 2 && i_start != N_ - 1) {
		if (j_end == N_ - 3) {
			winding_number_2 += 1;
		}
	}
	if ((j_end == j_start) && (j_end != N_ - 2)) {
		if (2 * j_start == - i_start + 2 *(N_ + Deg_ - 3) ) {
			if (i_end == i_start + 1) {
				winding_number_2 += 1;
			}
		}
		if (2 * j_end == - i_end + 2 *(N_ + Deg_ - 3) ) {
			if (i_start == i_end + 1) {
				winding_number_2 += 1;
			}
		}
	}

	if ( delta_i >= 2 * Deg_ - 1 && delta_j < (N_ - 2 )) {
		winding_number_1 += 1;
	}

}

void MonteCarlo::calculate_winding_strings() {
	winding_strings_[0] = 0;
	winding_strings_[1] = 0;
	// update vertical Spins
	int i = 0;
	for (int j = 0; j < Deg_; j++) {
		i = Deg_- j;
		if(S_->ifInsideLattice(i, j)) {
			this->update_winding_i_dir(0, i, j, 4, true);
		}
	}
	for (int j = Deg_; j < N_ - 2; j++) {
		i = 0;
		this->update_winding_i_dir(0, i, j, 5, false);
	}

	// update horizontal Spins

	int j = N_ - 2;
	for (int i = 0; i < N_ + Deg_ - j - 2; i++){
		if(S_->ifInsideLattice(i, j)) {
			this->update_winding_i_dir(1, i, j, 0, false);
		}
	}

	i = N_ - 2;
	for (int j = Deg_; j < N_ - 2; j++) {
		if(S_->ifInsideLattice(i, j)) {
			this->update_winding_i_dir(1, i, j, 4, true);
		}
		i = i - 1;
	}
}

void MonteCarlo::update_winding_i_dir(int index, int i, int j, int dir, bool plus) {
	DimerEdge* dimer = S_->get_Spin_pointer(i, j)->getDimer(dir);
	double d = dimer->getDimer();
	if (d == 1) {
		if(plus) {
			winding_strings_[index] += 1;
		} else {
			winding_strings_[index] -= 1;
		}
	}
}
