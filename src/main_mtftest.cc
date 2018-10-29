// main_mtftest.cc - Main function for mtftest
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_suffixtree.hh"
#include "bwtzip_file.hh"
#include "bwtzip_mtf.hh"
#include "bwtzip_zle.hh"
#include "bwtzip_arith.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "USAGE: mtftest infile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    cout << "Original size: " << v.size() << endl;
    bwtStree(v);
    cout << endl;
    const vector<unsigned char> orig = v;
    
    mtf(v); zleWheeler(v); arith(v);
    cout << "MTF: " << v.size() << endl;
    unarith(v); unzleWheeler(v); unmtf(v);
    if (v == orig) {
        cout << "MTF works." << endl;
    } else {
        cout << "FAILURE!" << endl;
    }

    mtf1(v); zleWheeler(v); arith(v);
    cout << "MTF-1: " << v.size() << endl;
    unarith(v); unzleWheeler(v); unmtf1(v);
    if (v == orig) {
        cout << "MTF-1 works." << endl;
    } else {
        cout << "FAILURE!" << endl;
    }

    mtf2(v); zleWheeler(v); arith(v);
    cout << "MTF-2: " << v.size() << endl;
    unarith(v); unzleWheeler(v); unmtf2(v);
    if (v == orig) {
        cout << "MTF-2 works." << endl;
    } else {
        cout << "FAILURE!" << endl;
    }

    ts0(v); zleWheeler(v); arith(v);
    cout << "TS0: " << v.size() << endl;
    unarith(v); unzleWheeler(v); unts0(v);
    if (v == orig) {
        cout << "TS0 works." << endl;
    } else {
        cout << "FAILURE!" << endl;
    }
}
