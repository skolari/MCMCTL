/*
 * MonteCarlo.cpp
 *
 *  Created on: 06.04.2018
 *      Author: Manuel Stathis
 */

#include "MonteCarlo.h"
using namespace std;

MonteCarlo::MonteCarlo(Random* Rnd, int Deg, int N_thermal, int N_algo, double J1, double J2, double J3, double Beta)
	: Rnd_(Rnd), worm_(), energy_measures_(), N_thermal_(N_thermal), N_algo_(N_algo)
{
	S_ = new SpinLattice(Rnd, Deg, J1, J2, J3, Beta);
	D_ = new DualLattice(Rnd, Deg, S_);
	entry_node_ = NULL;
	winding_number_horizontal = 0;
	winding_number_vertical = 0;

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
	/*
	if (d[0]->getDimer() == -1) {
		if (d[1]->getDimer() == -1) {
			if (d[2]->getDimer() == -1) {
				cerr << "this is not a spin configuration" << endl;
			}
		}
	}
	*/
	std::vector <long double> W = D_->get_local_weight(d0);
	std::vector< std::vector<long double>> M = this->get_M(W);
	std::vector<double> i{0, 1, 2, 3};
	std::vector<long double> w{M[0][0], M[0][1], M[0][2]};
	//std::cout << "M_0: " << M[0][0] << ", M_1: " << M[0][1] << ", M_2: " << M[0][2] << ", sum: " << M[0][0]+ M[0][1]+ M[0][2] << endl;
	std::piecewise_constant_distribution<double> dist(i.begin(), i.end(), w.begin());

	int next_index = int(Rnd_->piecewise_constant_distribution(dist));
	/*
	if ((next_index != 0) || (next_index != 1) || (next_index != 2)) {
		cout << "next_index : "<< next_index << endl;
		cout << "next_index is not int between 0 and 2" << endl;
	}
	*/
	if (next_index == 0) {
		next_edge = d[0]->getOppositeEdge();
	}

	else if (next_index == 1) {
		D_->switchDimer(d[0]);
		D_->switchDimer(d[1]);
		next_edge = d[1];
	}

	else if (next_index == 2) {
		D_->switchDimer(d[0]);
		D_->switchDimer(d[2]);
		next_edge = d[2];
	}

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
	//int count = 0;
	//int count_max = 20*S_->get_Number_spin();

	do {
		this->myopic_step();
		this->proba_step();
		last_edge = worm_.back();
		end_node = last_edge->getEnd();
		//count += 1;
	} while(end_node != entry_node_);

	//while(end_node != entry_node_ && count < count_max);

		/*
	if(count_max == count) {
		this->delete_worm();
		winding_number_horizontal = 0;
		winding_number_vertical = 0;
		cerr << "Too long worm." << endl;
		this->create_update();
	}
	*/

	if ((winding_number_horizontal % 2 == 0)
			&& (winding_number_vertical % 2 == 0)) {
		this->map_dimer_to_spin();
		winding_number_horizontal = 0;
		winding_number_vertical = 0;
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
			this->measure_energy();
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
	cout << "beta: "<<this->get_S()->get_Beta() << ", e_end: " << energy_measures_.back() << endl;
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

	for (int i = 0; i < 3; i ++) {
		if (i != i_max){
			W_other += W[i];
			m[j] = i;
			j += 1;
		}
	}
	//cout<< "W_max: " << W_max << ", W_other: " << W_other << endl;
	if (W_max > W_other) {
		A[i_max][i_max] = W[i_max] - W[m[0]] - W[m[1]];

		A[m[0]][i_max] = W[m[0]];
		A[i_max][m[0]] = A[m[0]][i_max];

		A[m[1]][i_max] = W[m[1]];
		A[i_max][m[1]] = A[m[1]][i_max];
		//cout << "W_max  > W_other" << endl;
	} else {
		// W_max <= W_other
		A[0][1] = 0.5 * (W[0] + W[1] - W[2]);
		A[1][0] = A[0][1];
		//cout << "A01 = " << A[0][1] <<  endl;
		A[0][2] = 0.5 * ( W[0] + W[2] - W[1]);
		A[2][0] = A[0][2];
		//cout << "A02 = " << A[0][2] <<  endl;

		A[1][2] = 0.5 * ( W[1] + W[2] - W[0]);
		A[2][1] = A[1][2];
		//cout << "A12 = " << A[1][2] <<  endl;

		//cout << "W_max =< W_other" << endl;
	}

	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			M[i][j] = A[i][j] / W[i];
		}
	}
	/*
	if ((M[0][0] + M[0][1] +M[0][2]) == 0)
	{
		cout <<"W0: " << W[0]<<" W1: "  << W[1]<<" W2: "  << W[2] << endl;
		std::cout << "M_0: " << M[0][0] << ", M_1: " << M[0][1] << ", M_2: " << M[0][2] << ", sum: " << M[0][0]+ M[0][1]+ M[0][2] << endl;

	}
	*/
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
void MonteCarlo::measure_energy() {
	double E = S_->get_energy_per_spin();
	energy_measures_.push_back(E);
}

/*
 * the first moment of the energy over all measurements (till now) of this simulation
 * @return first moment of the energy
 */
double MonteCarlo::first_moment_energy() {
	double energy_sum = 0;
	int N = energy_measures_.size();
	for (int i = 0; i < N; i++) {
	    energy_sum += energy_measures_[i];
	}
	energy_sum  = energy_sum / N;
	return energy_sum;
}

/*
 * the second moment of the energy over all measurements (till now) of this simulation
 * @return second moment of the energy
 */
double MonteCarlo::second_moment_energy() {
	double energy_sum = 0;
	int N = energy_measures_.size();
	for (int i = 0; i < N; i++) {
	    energy_sum += energy_measures_[i]*energy_measures_[i];
	}
	energy_sum  =energy_sum / N;
	return energy_sum;
}

double MonteCarlo::variance_energy() {
	double energy_first = first_moment_energy();
	double energy_second = second_moment_energy();
	return energy_second - energy_first * energy_first;
}

/*
 * Calculates the heat capacity (Cv) calculated wtih the first and second moment of the energy.
 * @return Cv
 */
double MonteCarlo::calculate_cv() {
	int Number_sites = S_->get_Number_spin();
	double first_moment_energy  = this->first_moment_energy();
	double second_moment_energy = this->second_moment_energy();
	double cv = Number_sites * (second_moment_energy - first_moment_energy * first_moment_energy) * S_->get_Beta() * S_->get_Beta();
	return cv;
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

	int j_last_edge_start = last_edge->getStart()->getPos(1);
	int j_last_edge_end = last_edge->getEnd()->getPos(1);
	int delta_j = std::abs(j_last_edge_start - j_last_edge_end);
	int i_last_edge_start = last_edge->getStart()->getPos(0);
	int i_last_edge_end = last_edge->getEnd()->getPos(0);
	int delta_i = std::abs(i_last_edge_start - i_last_edge_end);

	// vertical uptdate
	if ( delta_j >= (N_ - 2 )) {
		winding_number_vertical += 1;
	}
	//horizontal update
	else if ( delta_i >= 2 * Deg_ - 1) {
		winding_number_horizontal += 1;
	}
}


/*
 * this function can be used to test the winding number for Deg_ = 3
 */
void MonteCarlo::testwindingnumber() {
	for (int i = 0; i < N_ - 1; i++) {
		DimerNode* start = D_->getDimerNode(i , N_ - 2);
		DimerNode* end = D_->getDimerNode(i+1, N_ - 2);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber upperline (0,0):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}
	for (int i = 0; i < 3; i++) {
		DimerNode* start = D_->getDimerNode(2*i + 1, 5);
		DimerNode* end = D_->getDimerNode(2*i + 6, 0);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber up to down (0," << i+1 <<"):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}

	winding_number_horizontal = 0;
	winding_number_vertical = 0;
	for (int i = 0; i < 3; i++) {
		DimerNode* end = D_->getDimerNode(2*i + 1, 5);
		DimerNode* start = D_->getDimerNode(2*i + 6, 0);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber down to up (0," << i+1 <<"):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}

	winding_number_horizontal = 0;
	winding_number_vertical = 0;
	for (int i = 0; i < 3; i++) {
		DimerNode* start = D_->getDimerNode(6 + 2* i, 5 - i);
		DimerNode* end = D_->getDimerNode(1 + 2 * i, 2 - i);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber up right to down left ("<< i+1 <<",0):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}
	winding_number_horizontal = 0;
	winding_number_vertical = 0;
	for (int i = 0; i < 3; i++) {
		DimerNode* end = D_->getDimerNode(6 + 2* i, 5 - i);
		DimerNode* start = D_->getDimerNode(1 + 2 * i, 2 - i);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber down left to up right ("<< i+1 <<",0):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}

	winding_number_horizontal = 0;
	winding_number_vertical = 0;
	for (int i = 0; i < 3; i++) {
		DimerNode* end = D_->getDimerNode(0, Deg_ + i);
		DimerNode* start = D_->getDimerNode(2 * (N_ - 1) - 1, i);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber up left to down right ("<< i+1 <<",0):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}

	winding_number_horizontal = 0;
	winding_number_vertical = 0;

	for (int i = 0; i < 3; i++) {
		DimerNode* start = D_->getDimerNode(0, Deg_ + i);
		DimerNode* end = D_->getDimerNode(2 * (N_ - 1) - 1, i);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber down right to up left ("<< i+1 <<",0):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}

	winding_number_horizontal = 0;
	winding_number_vertical = 0;

	winding_number_horizontal = 0;
	winding_number_vertical = 0;
	for (int i = 0; i < 3; i++) {
		DimerNode* start = D_->getDimerNode(2 * (N_ - 1) - 2, i);
		DimerNode* end = D_->getDimerNode(2 * (N_ - 1) - 1, i);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber down right side (0,0):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}
	for (int i = 0; i < 3; i++) {
		DimerNode* start = D_->getDimerNode(2 * (N_ - 1) - 1, i);
		DimerNode* end = D_->getDimerNode(2 * (N_ - 1) - 2, i + 1);
		start->printoutPos();
		end->printoutPos();
		DimerEdge* edge = start->getEdge(end);
		worm_.push_back(edge);
		this->update_winding_number();
		cout << "windingnumber down right side (0,0):" << winding_number_horizontal << ", " << winding_number_vertical << endl;
	}

	cout << "test finished"<< endl;
}


