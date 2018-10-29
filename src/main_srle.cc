// main_srle.cc - Main function for srle
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_srle.hh"
#include "bwtzip_file.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: srle infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    srle(v);
    writeFile(v, argv[2]);
}
