// main_sha1test.cc - Main function for sha1test
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_file.hh"
#include "bwtzip_sha1.hh"
#include "bwtzip_common.hh"
#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace bwtzip;
using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        cout << "USAGE: sha1test infile [infile2 ...]" << endl;
        exit(EXIT_SUCCESS);
    }

    for (int z = 1; z < argc; z++) {
        try {
            const vector<unsigned char> hash = sha1(readFile(argv[z]));
    
            printf("%08lX %08lX %08lX %08lX %08lX %s\n",
                readUL(hash, 0), readUL(hash, 4), readUL(hash, 8),
                readUL(hash, 12), readUL(hash, 16), argv[z]);
        } catch (string e) {
        }
    }
}
