// bwtzip_sha1.cc - bwtzip SHA-1 functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_sha1.hh"
#include "bwtzip_common.hh"
using namespace std;
using namespace bwtzip;

namespace {
    vector<unsigned char> sha1Pad(const vector<unsigned char>& v) {
        // SHA-1 padding is simple.
        // First, 0x80 is appended to the data.
        // Then, 0x00 is appended until the byte length of the data mod 64 is 56.
        // Finally, the original number of BITS in the data is appended as an ULL.

        const vector<unsigned char> numbits = encodeULL((unsigned long long) v.size() * 8);
        vector<unsigned char> ret = v;
        ret.push_back(0x80);

        while (ret.size() % 64 != 56) {
            ret.push_back(0x00);
        }

        ret += numbits;
        return ret;
    }
}

vector<unsigned char> bwtzip::sha1(const vector<unsigned char>& v) {
    const vector<unsigned char> paddedv = sha1Pad(v);
    unsigned long a, b, c, d, e;
    vector<unsigned long> h(5);
    vector<unsigned long> w(80);
    unsigned long temp;

    h[0] = 0x67452301UL;
    h[1] = 0xEFCDAB89UL;
    h[2] = 0x98BADCFEUL;
    h[3] = 0x10325476UL;
    h[4] = 0xC3D2E1F0UL;

    for (size_t j = 0; j < paddedv.size(); j += 64) {
        for (int i = 0; i < 16; i++) {
            w[i] = readUL(paddedv, j + i * 4);
        }

        for (int t = 16; t < 80; t++) {
            w[t] = w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16];
            w[t] = w[t] << 1 | w[t] >> 31;
        }

        a = h[0]; b = h[1]; c = h[2]; d = h[3]; e = h[4];

        for (int t = 0; t < 20; t++) {
            temp = (a << 5 | a >> 27) + (b & c | ~b & d) + e + w[t] + 0x5A827999UL;
            e = d; d = c; c = b << 30 | b >> 2; b = a; a = temp;
        }
        for (int t = 20; t < 40; t++) {
            temp = (a << 5 | a >> 27) + (b ^ c ^ d) + e + w[t] + 0x6ED9EBA1UL;
            e = d; d = c; c = b << 30 | b >> 2; b = a; a = temp;
        }
        for (int t = 40; t < 60; t++) {
            temp = (a << 5 | a >> 27) + (b & c | b & d | c & d) + e + w[t] + 0x8F1BBCDCUL;
            e = d; d = c; c = b << 30 | b >> 2; b = a; a = temp;
        }
        for (int t = 60; t < 80; t++) {
            temp = (a << 5 | a >> 27) + (b ^ c ^ d) + e + w[t] + 0xCA62C1D6UL;
            e = d; d = c; c = b << 30 | b >> 2; b = a; a = temp;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
    }

    vector<unsigned char> hash;
    hash.reserve(20);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            hash.push_back(decomposeUL(h[i], j));
        }
    }

    return hash;
}
