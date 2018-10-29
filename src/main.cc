// main.cc - Main function
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_file.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: main infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    // Put code here
    writeFile(v, argv[2]);
}
