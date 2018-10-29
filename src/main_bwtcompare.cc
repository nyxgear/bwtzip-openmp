// main_bwtcompare.cc - Main function for bwtcompare
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
#include "bwtunzip.hh"
#include "bwtzip_suffixtree.hh"
#include "bwtzip_mtf.hh"
#include "bwtzip_zle.hh"
#include "bwtzip_arith.hh"
#include "bwtzip_huff.hh"
#include "bwtzip_ahuff.hh"
#include "bwtzip_file.hh"
#include "clock.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "USAGE: bwtcompare infile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    const vector<unsigned char> orig = v;

    cout << "Original file size: " << v.size() << endl;
    bwtStree(v);
    mtf(v); huff(v); 
	cout << "BWT / MTF / Huff: " << v.size() << endl;
    puff(v); ahuff(v);
	cout << "BWT / MTF / Ahuff: " << v.size() << endl;
    unahuff(v); arith(v);
	cout << "BWT / MTF / Arith: " << v.size() << endl;
    unarith(v); unmtf(v); mtf2(v); arith(v);
	cout << "BWT / MTF-2 / Arith: " << v.size() << endl;
    unarith(v); zleWheeler(v); arith(v);
	cout << "BWT / MTF-2 / Wheeler ZLE / Arith: " << v.size() << endl;
    unarith(v);

    Clock hufftimer("Huff Timer");
    huff(v);
    hufftimer.print();
	cout << "BWT / MTF-2 / Wheeler ZLE / Huff: " << v.size() << endl;
    Clock pufftimer("Puff Timer");
    puff(v);
    pufftimer.print();

    ahuff(v);
	cout << "BWT / MTF-2 / Wheeler ZLE / Ahuff: " << v.size() << endl;
    unahuff(v); unzleWheeler(v); unmtf2(v); unbwt(v);	

    if (v == orig) {
        cout << "Success!" << endl;
    } else {
        cout << "FAILURE!" << endl;
    }
}
