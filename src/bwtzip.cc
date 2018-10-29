// bwtzip.cc - bwtzip functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
using namespace std;

vector<size_t> bwtzip::convertPosToLengths(const vector<size_t>& pos) {
    const size_t n = pos.size();

    vector<size_t> l(n);

    for (size_t i = 0; i < n; i++) {
        l[i] = n - pos[i];
    }

    return l;
}
