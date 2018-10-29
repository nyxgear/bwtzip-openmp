// bwtzip_bzip2.cc - bwtzip bzip2 functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_bzip2.hh"
#include <bzlib.h>
#include <iostream>
#include <cstdlib>
using namespace std;

vector<unsigned char>& bwtzip::bzip2(vector<unsigned char>& v) {
    unsigned int destLen = v.size() + (v.size() >> 6) + 600;
    vector<unsigned char> dest(destLen);

    int error = BZ2_bzBuffToBuffCompress((char *) &dest[0], &destLen, (char *) &v[0], v.size(), 9, 0, 0);

    if (error != BZ_OK) {
        cout << "libbzip reported an error." << endl;
        exit(EXIT_FAILURE);
    }

    dest.resize(destLen); // Upon exit, destLen is the actual size of the compressed buffer.
    swap(v, dest);
    return v;
}

vector<unsigned char>& bwtzip::unbzip2(vector<unsigned char>& v, size_t originalsize) {
    unsigned int destLen = originalsize;
    vector<unsigned char> dest(destLen);

    int error = BZ2_bzBuffToBuffDecompress((char *) &dest[0], &destLen, (char *) &v[0], v.size(), 0, 0);

    if (error != BZ_OK) {
        cout << "libbzip reported an error." << endl;
        exit(EXIT_FAILURE);
    }

    dest.resize(destLen); // Upon exit, destLen is the actual size of the decompressed buffer.    
    swap(v, dest);
    return v;
}
