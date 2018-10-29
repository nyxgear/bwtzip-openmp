// bwtzip_suffixbentleysedgewick.cc - bwtzip Bentley-Sedgewick Suffix Sort functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_common.hh"
#include "bwtzip_suffixbentleysedgewick.hh"
#include <stack>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::bentleysedgewick;

int bwtzip::bentleysedgewick::medianOfThree(int a, int b, int c) {
    if (a < b) {
        if (b < c) {        // A B C
            return b;
        } else if (a < c) { // A C B
            return c;
        } else {            // C A B
            return a;
        }
    } else if (c < b) {     // C B A
        return b;
    } else if (a < c) {     // B A C
        return a;
    } else {                // B C A
        return c;
    }
}

int bwtzip::bentleysedgewick::findMedian(const vector<pair<const int *, const int *> >& v, size_t l, size_t r) {
    const int a = *v[l].first;
    const int b = *v[r - 1].first;
    const int c = *v[(l + r - 1) / 2].first;
    return medianOfThree(a, b, c);
}

void bwtzip::bentleysedgewick::triPartition(vector<pair<const int *, const int *> >& v) {
    stack<size_t> lstak, rstak;
    lstak.push(0);
    rstak.push(v.size());

    while (!lstak.empty()) {
        const size_t l = lstak.top(); lstak.pop();
        const size_t r = rstak.top(); rstak.pop();

        const int mediansymbol = findMedian(v, l, r);

        vector<pair<const int *, const int *> > equalto, greaterthan;
        equalto.reserve(r - l);
        greaterthan.reserve(r - l);
        size_t pos = l;

        for (size_t i = l; i < r; i++) {
            if (*v[i].first < mediansymbol) {
                v[pos++] = v[i];
            } else if (*v[i].first > mediansymbol) {
                greaterthan.push_back(v[i]);
            } else {
                equalto.push_back(make_pair(v[i].first + 1, v[i].second));
            }
        }

        const size_t secondpart = pos + equalto.size();

        for (size_t i = 0; i < equalto.size(); i++) {
            v[pos + i] = equalto[i];
        }
        for (size_t i = 0; i < greaterthan.size(); i++) {
            v[secondpart + i] = greaterthan[i];
        }

        if (pos - l > 1) {
            lstak.push(l); rstak.push(pos);
        }
        if (equalto.size() > 1) {
            lstak.push(pos); rstak.push(secondpart);
        }
        if (greaterthan.size() > 1) {
            lstak.push(secondpart); rstak.push(r);
        }
    }
}

vector<unsigned char>& bwtzip::bwtBentleySedgewick(vector<unsigned char>& v) {
    vector<int> sentineled(v.begin(), v.end());
    sentineled.push_back(SENTINEL);

    vector<pair<const int *, const int *> > pos(sentineled.size());
    for (size_t i = 0; i < pos.size(); i++) {
        pos[i] = make_pair(&sentineled[i], &sentineled[i]);
    }

    triPartition(pos);

    vector<unsigned char> transformed(sentineled.size());
    vector<unsigned char> primaryindex;
    vector<unsigned char> sentinelindex;

    const int * const secondloc = &sentineled[1];

    for (size_t i = 0; i < transformed.size(); i++) {
        const int * const p = pos[i].second;

        if (p > secondloc) {
            transformed[i] = p[-1];
        } else if (p == secondloc) {
            primaryindex = encodeUL(i);
            transformed[i] = p[-1];
        } else {
            sentinelindex = encodeUL(i);
            transformed[i] = 0; // Dummy character
        }
    }

    v = primaryindex + sentinelindex + transformed;
    return v;
}
