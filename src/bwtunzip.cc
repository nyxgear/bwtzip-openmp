// bwtunzip.cc - bwtunzip functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtunzip.hh"
#include "bwtzip_common.hh"
#include "bwtzip_mtf.hh"
#include "bwtzip_zle.hh"
#include "bwtzip_arith.hh"
#include "clock.hh"
#include <iostream>
#include <cstdlib>
#include <bwtunzip.hh>
#include <bwtzip_file.hh>

using namespace std;
using namespace bwtzip;

vector<unsigned char>& bwtzip::bwtDecompress(vector<unsigned char>& v) {
    clockStart();
    unarith(v);      cout << "UnArith:       " << clockReport() << endl;
    unzleWheeler(v); cout << "UnWheeler ZLE: " << clockReport() << endl;
    unmtf2(v);       cout << "UnMTF-2:       " << clockReport() << endl;
    unbwt(v);        cout << "UnBWT:         " << clockReport() << endl;

    return v;
}

vector<unsigned char>& bwtzip::unbwt(vector<unsigned char>& v) {
    size_t index = readUL(v, 0);                // Read primary index
    size_t sentinelindex = readUL(v, 4);        // Read sentinel index

    v.erase(v.begin(), v.begin() + 8);          // Remove encoded indices
    vector<unsigned char> decoded(v.size());    // Holds original data

    vector<size_t> freqs(257, 0);               // Fenwick's K

    for (size_t i = 0; i < v.size(); i++) {
        if (i == sentinelindex) {
            freqs[256]++;
        } else {
            freqs[v[i]]++;
        }
    }

    vector<size_t> links(v.size());         // Fenwick's L
    vector<size_t> mapping(257);            // Fenwick's M

    mapping[0] = 0;                         // Build M
    for (int j = 1; j < 257; j++) {
        mapping[j] = mapping[j - 1] + freqs[j - 1];
    }

    for (size_t i = 0; i < v.size(); i++) { // Build L
        if (i == sentinelindex) {
            links[i] = mapping[256]++;
        } else {
            links[i] = mapping[v[i]]++;
        }
    }

    for (long int i = v.size() - 1; i >= 0; i--) {  // Perform Reverse-BWT
        index = links[index];
        decoded[i] = v[index];
    }

    decoded.pop_back(); // Remove sentinel character
    swap(v, decoded);
    return v;
}


void bwtzip::bwtunzipMain(int argc, char **argv) {
    if (argc != 3) {
        cout << "USAGE: bwtunzip infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    InputFile infile(argv[1]);
    if (infile.extractAtMost(8) != encodeULL(BWTZIP_SIG)) {
        cout << "bwtzip signature not found." << endl;
        exit(EXIT_FAILURE);
    }

    OutputFile outfile(argv[2]);

    while (true) {
        const vector<unsigned char> length = infile.extractAtMost(4);
        if (length.empty()) {
            break;
        }
        const vector<unsigned char> id = infile.extractAtMost(1);
        const unsigned long lengthnum = readUL(length, 0);
        vector<unsigned char> v = infile.extractAtMost(lengthnum);

        if (id[0] == 1) {
            bwtDecompress(v);
        }

        outfile.append(v);
    }
}
