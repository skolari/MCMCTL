/*
 * Spin.h
 *
 *  Created on: 28.03.2018
 *      Author: Manuel Stathis
 */

#ifndef SPIN_H_
#define SPIN_H_
#include <vector>
#include <cassert>

struct Spin{
    const int q_, r_, spin_;
    std::vector<Spin*> neighbors_;

    Spin(int q, int r, int s, double spin, std::vector<Spin*> neighbors)
    :	q_(q), r_(r), spin_(spin), neighbors_(neighbors)
    {
        assert (q + r + s == 0);
    }
    Spin(int q, int r, double spin, std::vector<Spin*> neighbors)
    : q_(q), r_(r), spin_(spin), neighbors_(neighbors)
    {
    }

    inline int q() { return q_; }
    inline int r() { return r_; }
    inline int s() { return - q_ - r_; }
    inline int spin() { return spin_; }
    inline Spin* neighbor(int d) { return neighbors_(d); }
};

// if Spin at the same position
bool operator == (Spin a, Spin b) {
    return a.q == b.q && a.r == b.r && a.s == b.s;
}


/**
std::vector<int> Spin_neighbor(Spin a, int i) {
    return [a.q + b.q, a.r + b.r, a.s + b.s];
}

Hex hex_multiply(Hex a, int k) {
    return Hex(a.q * k, a.r * k, a.s * k);
*/
}

#endif /* SPIN_H_ */
