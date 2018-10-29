// bwtzip_zlib.cc - bwtzip zlib functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_zlib.hh"
#include <iostream>
#include <cstdlib>
#include <zlib.h>
using namespace std;

namespace {
    // Modified compress2() function from zlib that calls deflateinit2().
    // It has the power of init2 with the convenience of compress.
    int myCompress(
        unsigned char *dest, unsigned long int *destlen,
        const unsigned char *source, unsigned long int sourcelen,
        int num_strategy) {

        z_stream the_stream;
        int err;

        the_stream.next_in = (unsigned char *) source;
        the_stream.avail_in = (unsigned int) sourcelen;
        the_stream.next_out = dest;
        the_stream.avail_out = (unsigned int) *destlen;

        if ((unsigned long) the_stream.avail_out != *destlen) {
            return Z_BUF_ERROR;
        }

        the_stream.zalloc = (alloc_func)0;
        the_stream.zfree = (free_func)0;
        the_stream.opaque = (voidpf)0;

        // level 9, windowBits 15, memLevel 9
        err = deflateInit2(&the_stream, 9, Z_DEFLATED, 15, 9, num_strategy);

        if (err != Z_OK) {
            return err;
        }

        if ((err = deflate(&the_stream, Z_FINISH)) != Z_STREAM_END) {
            deflateEnd(&the_stream);
            return ((err == Z_OK) ? Z_BUF_ERROR : err);
        }

        *destlen = the_stream.total_out;
        return deflateEnd(&the_stream);
    }

    vector<unsigned char> compressStrategy(const vector<unsigned char>& v, int num_strategy) {
        unsigned long destlen = v.size() + (v.size() >> 9) + 12; // Slightly more than required by zlib

        vector<unsigned char> dest(destlen);

        int error = myCompress(&dest[0], &destlen, &v[0], v.size(), num_strategy);

        if (error != Z_OK) {
            cout << "zlib reported an error." << endl;
            exit(EXIT_FAILURE);
        }

        dest.resize(destlen); // Upon exit, destlen is the actual size of the compressed buffer.
        return dest;
    }
}

vector<unsigned char>& bwtzip::compress(vector<unsigned char>& v) {
    vector<unsigned char> t1 = compressStrategy(v, Z_DEFAULT_STRATEGY);
    vector<unsigned char> t2 = compressStrategy(v, Z_FILTERED);
    vector<unsigned char> t3 = compressStrategy(v, Z_HUFFMAN_ONLY);

    if (t1.size() <= t2.size() && t1.size() <= t3.size()) {
        swap(v, t1);
    } else if (t2.size() <= t1.size() && t2.size() <= t3.size()) {
        swap(v, t2);
    } else {
        swap(v, t3);
    }

    return v;
}

vector<unsigned char>& bwtzip::decompress(vector<unsigned char>& v, size_t originalsize) {
    unsigned long destlen = originalsize;
    vector<unsigned char> dest(destlen);

    int error = uncompress(&dest[0], &destlen, &v[0], v.size());

    if (error != Z_OK) {
        cout << "zlib reported an error." << endl;
        exit(EXIT_FAILURE);
    }

    dest.resize(destlen);
    swap(v, dest);
    return v;
}
