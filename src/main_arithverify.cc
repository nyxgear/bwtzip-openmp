// main_arithverify.cc - Main function for arithverify
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_arith.hh"
#include "bwtzip_file.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "USAGE: arithverify infile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    const vector<unsigned char> orig = v;
    arith(v);
    unarith(v);
    if (orig == v) {
        cout << "Arithmetic coding is lossless!" << endl;
    } else {
        cout << "Arithmetic coding FAILED!" << endl;
    }
}
