// bwtzip_suffixstlsort.cc - bwtzip Suffix STL Sort functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
#include "bwtzip_common.hh"
#include "bwtzip_suffixstlsort.hh"
#include "clock.hh"
#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::stlsort;

vector<size_t> bwtzip::stlsort::computePos(const vector<short>& v) {
    vector<size_t> pos(v.size());
    for (size_t i = 0; i < pos.size(); i++) {
        pos[i] = i;
    }

    Comparator c(&v[0]);
    sort(pos.begin(), pos.end(), c);
    return pos;
}

vector<unsigned char>& bwtzip::bwtSuffixStlSort(vector<unsigned char>& v) {
    vector<short> sentineled(v.begin(), v.end());
    sentineled.push_back(SENTINEL);

    const vector<size_t> l = convertPosToLengths(computePos(sentineled));
    vector<unsigned char> transformed(sentineled.size());

    vector<unsigned char> primaryindex;
    vector<unsigned char> sentinelindex;

    for (size_t i = 0; i < l.size(); i++) {
        if (l[i] < v.size()) {
            transformed[i] = v[v.size() - l[i]];
        } else if (l[i] == v.size()) {
            primaryindex = encodeUL(i);
            transformed[i] = v[0];
        } else { // if (l[i] == v.size() + 1)
            sentinelindex = encodeUL(i);
            transformed[i] = 0; // Dummy character
        }
    }

    v = primaryindex + sentinelindex + transformed;
    return v;
}
