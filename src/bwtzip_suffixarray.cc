// bwtzip_suffixarray.cc - bwtzip Suffix Array functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
#include "bwtzip_common.hh"
#include "bwtzip_suffixarray.hh"
#include "clock.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::manbermyers;

// mtent13.txt
// BWT: 515.11  with vector<T>
// BWT: 422.453 with bare arrays

vector<size_t> bwtzip::manbermyers::computePos(const vector<short>& a) {
    const size_t n = a.size();
    int * const pos = new int[n];
    int * const prm = new int[n];
    int * const count = new int[n];
    bool * const bh = new bool[n + 1];
    bool * const b2h = new bool[n + 1];

    // Begin first phase bucket sort

    {
        for (short k = 0; k < 257; k++) {
            pos[k] = -1;
        }

        for (size_t i = 0; i < n; i++) {
            prm[i] = pos[a[i]];
            pos[a[i]] = i;
        }

        int c = 0;

        for (short k = 0; k < 257; k++) {
            int i = pos[k];
            while (i != -1) {
                int j = prm[i];
                prm[i] = c;
                bh[c] = (i == pos[k]);
                ++c;
                i = j;
            }
        }

        bh[n] = true;
        for (size_t i = 0; i < n; i++) {
            pos[prm[i]] = i;
        }
    }

    // End of first phase bucket sort
    // Begin inductive sorting

    for (size_t h = 1; h < n; h *= 2) {
        {
            size_t l = 0;

            for (size_t i = 0; i < n; i++) {
                if (bh[i]) {
                    l = i;
                    count[l] = 0;
                }
                prm[pos[i]] = l;
            }
        }

        size_t d = n - h;
        size_t e = prm[d];
        b2h[prm[d] = e + count[e]++] = true;

        for (size_t i = 0; i < n; i++) {
            if (bh[i]) {    // We are at the start of a bucket
                size_t l = i;
                size_t r = i;
                for (size_t j = i + 1; j < n + 1; j++) {
                    if (bh[j]) {
                        r = j - 1;
                        break;
                    }
                }
                // The bucket is [l, r]
                for (size_t c = l; c <= r; c++) {
                    if ((size_t) pos[c] >= h && (size_t) pos[c] <= n + h - 1) {
                        size_t dd = pos[c] - h;
                        e = prm[dd];
                        b2h[prm[dd] += count[e]++] = true;
                    }
                }

                for (size_t c = l; c <= r; c++) {
                    if ((size_t) pos[c] >= h && (size_t) pos[c] <= n + h - 1) {
                        size_t dd = pos[c] - h;

                        if (b2h[prm[dd]]) {
                            for (size_t j = prm[dd] + 1; ; j++) {
                                if (bh[j] || !b2h[j]) {
                                    e = j;
                                    break;
                                }
                            }
                            for (size_t f = prm[dd] + 1; f < e; f++) {
                                b2h[f] = false;
                            }
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < n; i++) {
            pos[prm[i]] = i;
            if (b2h[i]) {
                bh[i] = true;
            }
        }
    }

    // End inductive sorting; pos is finished
    vector<size_t> ret(pos, pos + n);
    delete[] pos;
    delete[] prm;
    delete[] count;
    delete[] bh;
    delete[] b2h;
    return ret;
}

vector<unsigned char>& bwtzip::bwtSarray(vector<unsigned char>& v) {
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
