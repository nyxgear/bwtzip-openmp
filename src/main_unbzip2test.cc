// main_unbzip2test.cc - Main function for unbzip2test
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include <iostream>
#include <cstdlib>
#include "bwtzip_file.hh"
#include "bwtzip_bzip2.hh"
#include "bwtzip_common.hh"
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: unbzip2test infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    const unsigned long size = readUL(v, 0);
    v.erase(v.begin(), v.begin() + 4);
    unbzip2(v, size);
    writeFile(v, argv[2]);
}
