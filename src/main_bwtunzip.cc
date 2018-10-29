// main_bwtunzip.cc - Main function for bwtunzip
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtunzip.hh"
#include "bwtzip_file.hh"
#include "bwtzip_common.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: bwtunzip infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    InputFile infile(argv[1]);
    if (infile.extractAtMost(8) != encodeULL(BWTZIP_SIG)) {
        cout << "bwtzip signature not found." << endl;
        exit(EXIT_FAILURE);
    }

    OutputFile outfile(argv[2]);

    while (true) {
        const vector<unsigned char> length = infile.extractAtMost(4);
        if (length.empty()) {
            break;
        }
        const vector<unsigned char> id = infile.extractAtMost(1);
        const unsigned long lengthnum = readUL(length, 0);
        vector<unsigned char> v = infile.extractAtMost(lengthnum);

        if (id[0] == 1) {
            bwtDecompress(v);
        }

        outfile.append(v);
    }
}
