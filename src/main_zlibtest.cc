// main_zlibtest.cc - Main Function For zlibtest
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include <iostream>
#include <cstdlib>
#include "bwtzip_file.hh"
#include "bwtzip_zlib.hh"
#include "bwtzip_common.hh"
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: zlibtest infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    const size_t size = v.size();
    compress(v);
    v = encodeUL(size) + v;
    writeFile(v, argv[2]);
}
