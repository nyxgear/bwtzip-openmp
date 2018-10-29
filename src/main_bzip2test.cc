/* main_bzip2test.cc - Main function for bzip2test */
/* GPLed - Stephan T. Lavavej - http://stl.caltech.edu */

#include <iostream>
#include <cstdlib>
#include "bwtzip_file.hh"
#include "bwtzip_bzip2.hh"
#include "bwtzip_common.hh"
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: bzip2test infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    const size_t size = v.size();
    bzip2(v);
    v = encodeUL(size) + v;
    writeFile(v, argv[2]);
}
