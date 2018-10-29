// sha1prng.hh - SHA-1 PseudoRandom Number Generator
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef SHA1PRNG_HH
#define SHA1PRNG_HH

#include "bwtzip_common.hh"
#include <list>
using namespace std;
using namespace bwtzip;

class Sha1Prng {
public:
    Sha1Prng(unsigned long ul0 = 0, unsigned long ul1 = 0, unsigned long ul2 = 0,
        unsigned long ul3 = 0, unsigned long ul4 = 0) 
        : ul0(ul0), ul1(ul1), ul2(ul2), ul3(ul3), ul4(ul4), loc(5) { }
    unsigned char getRandomUC();
    unsigned long getRandomUL();

private:
    struct Hash {
        Hash() : a(0), b(0), c(0), d(0), e(0) { }
        Hash(unsigned long a, unsigned long b, unsigned long c, unsigned long d,
            unsigned long e) : a(a), b(b), c(c), d(d), e(e) { }
        unsigned long a, b, c, d, e;
    };

    void incrementCounter() { !++ul4 && !++ul3 && !++ul2 && !++ul1 && ++ul0; }
    const Hash hashCounter() const;

    unsigned long ul0, ul1, ul2, ul3, ul4;
    list<unsigned char> randomdata;
    Hash randomhash;
    size_t loc;
};

unsigned char Sha1Prng::getRandomUC() {
    if (randomdata.empty()) { // Need to get some more random numbers
        const Hash h = hashCounter();
        incrementCounter();
        for (int t = 0; t < 4; t++)
            randomdata.push_back(decomposeUL(h.a, t));
        for (int t = 0; t < 4; t++)
            randomdata.push_back(decomposeUL(h.b, t));
        for (int t = 0; t < 4; t++)
            randomdata.push_back(decomposeUL(h.c, t));
        for (int t = 0; t < 4; t++)
            randomdata.push_back(decomposeUL(h.d, t));
        for (int t = 0; t < 4; t++)
            randomdata.push_back(decomposeUL(h.e, t));
    }

    const unsigned char ret = randomdata.front();
    randomdata.pop_front();
    return ret;
}

unsigned long Sha1Prng::getRandomUL() {
    if (loc == 5) { // Need to get some more random numbers
        randomhash = hashCounter();
        incrementCounter();
        loc = 0;
    }

    switch (loc++) {
    case 0: return randomhash.a;
    case 1: return randomhash.b;
    case 2: return randomhash.c;
    case 3: return randomhash.d;
    case 4: return randomhash.e;
    default: exit(EXIT_FAILURE);
    }
}

const Sha1Prng::Hash Sha1Prng::hashCounter() const {
    unsigned long w[80], temp;

    unsigned long a = 0x67452301UL;
    unsigned long b = 0xEFCDAB89UL;
    unsigned long c = 0x98BADCFEUL;
    unsigned long d = 0x10325476UL;
    unsigned long e = 0xC3D2E1F0UL;

    w[0] = ul0; w[1] = ul1; w[2] = ul2; w[3] = ul3; w[4] = ul4;
    w[5] = 0x80000000UL;
    w[15] = 0x000000A0UL;

    for (int t = 6; t < 15; t++)
        w[t] = 0;
    
    for (int t = 16; t < 80; t++) {
        w[t] = w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16];
        w[t] = (w[t] << 1 | w[t] >> 31);
    }

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

    return Hash(a + 0x67452301UL, b + 0xEFCDAB89UL, c + 0x98BADCFEUL,
        d + 0x10325476UL, e + 0xC3D2E1F0UL);
}

#endif // End of: #ifndef SHA1PRNG_HH
