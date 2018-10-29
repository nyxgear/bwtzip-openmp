// main_sha512test.cc - Main function for sha512test
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_file.hh"
#include "bwtzip_sha512.hh"
#include "bwtzip_common.hh"
#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace bwtzip;
using namespace std;

namespace {
    void printULL(unsigned long long x) {
        for (int i = 0; i < 8; i++) {
            printf("%02X", decomposeULL(x, i));
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        cout << "USAGE: sha512test infile [infile2 ...]" << endl;
        exit(EXIT_SUCCESS);
    }

    for (int z = 1; z < argc; z++) {
        try {
            const vector<unsigned long long> hash = sha512(readFile(argv[z]));

            for (int i = 0; i < 8; i++) {
                printULL(hash[i]);
                if (i == 3 || i == 7) {
                    printf("\n");
                } else {
                    printf(" ");
                }
            }
            printf("%s\n\n", argv[z]);
        } catch (string e) {
        }
    }
}
