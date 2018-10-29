// main_huffverify.cc - Main function for huffverify
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_huff.hh"
#include "bwtzip_file.hh"
#include "clock.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "USAGE: huffverify infile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    const vector<unsigned char> orig = v;
cout << "Original size: " << orig.size() << endl;
Clock hufftimer("Huff Timer");
    huff(v);
hufftimer.print();
cout << "Huffed size: " << v.size() << endl;
Clock pufftimer("Puff Timer");
    puff(v);
pufftimer.print();
    if (orig == v) {
        cout << "Huffman coding is lossless!" << endl;
    } else {
        cout << "Huffman coding FAILED!" << endl;

        for (size_t i = 0; i < v.size(); i++) {
            cout << "i: " << i << " | v[i]: " << (int) v[i] << endl;
        }
        for (size_t i = 0; i < orig.size(); i++) {
            cout << "i: " << i << " | orig[i]: " << (int) orig[i] << endl;
        }
    }
}
