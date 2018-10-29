// main_unsrle.cc - Main function for unsrle
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_srle.hh"
#include "bwtzip_file.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: unsrle infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    unsrle(v);
    writeFile(v, argv[2]);
}
