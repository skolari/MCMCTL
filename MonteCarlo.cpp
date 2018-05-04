/*
 * MonteCarlo.cpp
 *
 *  Created on: 06.04.2018
 *      Author: Manuel Stathis
 */

#include "MonteCarlo.h"
using namespace std;
int check_if_correct_update(std::vector<DimerEdge*> d);

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

	if (entry_node_->getEdge(rnd)->getDimer() == 0) {
		cerr <<"entry_node has dimer0" << endl;
	}

	this->update_winding_number();
	this->proba_step();
}

// maybe there is a more efficient way.
void MonteCarlo::myopic_step() {
	DimerEdge* last_edge = worm_.back();

	vector<DimerEdge*> d = D_->get_local_dimer(last_edge);

	DimerNode* end_node = last_edge->getEnd();
	DimerNode* start_node = last_edge->getStart();
	DimerNode* new_end = end_node;
	DimerEdge* next_edge = last_edge;
	double rnd = 0;

	do{ // maybe create function, edges_without(dimerEdge*)
		rnd = Rnd_->dist_3();
		next_edge = end_node->getEdge(rnd);
		new_end = next_edge->getEnd();
	} while(new_end == start_node);

	worm_.push_back(next_edge);
	this->update_winding_number();

}

void MonteCarlo::proba_step() {

	DimerEdge* d0 = worm_.back();
	DimerEdge* next_edge = NULL;

	std::vector< DimerEdge* > d = D_->get_local_dimer(d0);
	/*
	if (d[0]->getDimer() == -1) {
		if (d[1]->getDimer() == -1) {
			if (d[2]->getDimer() == -1) {
<<<<<<< HEAD
				this->Printout("./Debugg/");

=======
				/*
>>>>>>> new_try_server
				cout << "positions:"<< endl;
				for (int k = 0; k<3 ; k++ ) {
					cout << "start: ";
					d[k]->getStart()->printoutPos();
					cout << "end: ";
					d[k]->getEnd()->printoutPos();
				}

				cerr << "this is not a spin configuration" << endl;
			}
		}
	}
	*/
	std::vector <long double> W = D_->get_local_weight(d0);
	std::vector< std::vector<long double>> M = this->get_M(W);
	std::vector<double> i{0, 1, 2, 3};
	std::vector<long double> w{M[0][0], M[0][1], M[0][2]};
<<<<<<< HEAD
	//std::cout << "M[0][0]: " << M[0][0] << ", M[0][1]: " << M[0][1] << ", M[0][2]: " << M[0][2] << ",sum: "<< M[0][0] + M[0][1] + M[0][2] << std::endl;
	std::piecewise_constant_distribution<double> dist(i.begin(), i.end(), w.begin());

	int next_index = Rnd_->piecewise_constant_distribution(dist); /*
=======
	std::piecewise_constant_distribution<double> dist(i.begin(), i.end(), w.begin());

	int next_index = Rnd_->piecewise_constant_distribution(dist);
	/*
>>>>>>> new_try_server
	if (next_index != 0 || next_index != 1 || next_index != 2) {
		cout << "next_index : "<< next_index << endl;
		cout << "next_idendex is not int between 0 and 2" << endl;
	}
<<<<<<< HEAD
*/
=======
	*/

>>>>>>> new_try_server
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

/*
 * 	d = D_->get_local_dimer(d0);
 *
		if (d[0]->getDimer() == -1) {
			if (d[1]->getDimer() == -1) {
				if (d[2]->getDimer() == -1) {
					this->Printout("./Debugg/");
					cout << "starti="<< d0->getStart()->getPos(0) << ", j ="<< d0->getStart()->getPos(1)<< endl;
					cout << "endi="<< d0->getEnd()->getPos(0) << ", j ="<< d0->getEnd()->getPos(1)<< endl;
					cout << "end: " << endl;
					cerr << "this is not a spin configuration" << endl;
				}
			}
		}
		*/
}


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

	if (W_max <= W_other) {
		A[0][1] = 0.5 * (W[0] + W[1] - W[2]);
		A[1][0] = A[0][1];

		A[0][2] = 0.5 * ( W[0] + W[2] - W[1]);
		A[2][0] = A[0][2];

		A[1][2] = 0.5 * ( W[1] + W[2] - W[0]);
		A[2][1] = A[1][2];
	} else {
		A[i_max][i_max] = W_max - W_other;

		A[m[0]][i_max] = W[m[0]];
		A[i_max][m[0]] = A[m[0]][i_max];

		A[m[1]][i_max] = W[m[1]];
		A[i_max][m[1]] = A[m[1]][i_max];
	}

	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			M[i][j] = A[i][j] / W[i];
		}
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
	for (auto& n : energy_measures_)
	    energy_sum += n;
	return energy_sum / N;
}

/*
 * the second moment of the energy over all measurements (till now) of this simulation
 * @return second moment of the energy
 */
double MonteCarlo::second_moment_energy() {
	double energy_sum = 0;
	int N = energy_measures_.size();
	for (auto& n : energy_measures_)
	    energy_sum += n*n;
	return energy_sum / N;
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

int check_if_correct_update(std::vector<DimerEdge*> d) {
	// case 1: no correct configuration.
	if(d[0]->getDimer() == -1) {
		if(d[1]->getDimer() == -1 ) {
			if(d[2]->getDimer() == -1) {
				//return 1;
			}
		}
	}

	//case 2: no correct connection
	DimerNode* d0end = d[0]->getEnd();
	DimerNode* d1start = d[1]->getStart();
	DimerNode* d2start = d[2]->getStart();
	if((d0end->getPos(0) != d1start->getPos(0)) || (d0end->getPos(1) != d1start->getPos(1)) || (d0end->getPos(0) != d2start->getPos(0)) || (d0end->getPos(1) != d2start->getPos(1)) ) {
		return 2;
	}
	return 0;
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


