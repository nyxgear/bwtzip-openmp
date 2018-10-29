// main_prngtest.cc - Main function for prngtest
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "clock.hh"
#include "sha1prng.hh"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "USAGE: prngtest numbytes" << endl;
        exit(EXIT_SUCCESS);
    }

    const size_t len = atoi(argv[1]);
    Sha1Prng prng;

    cout << "Generating unsigned longs." << endl;

    unsigned long * const data = new unsigned long[len];

    clockStart();

    for (size_t i = 0; i < len; i++) {
        data[i] = prng.getRandomUL();
    }

    clockPrint();

    delete[] data;
}
