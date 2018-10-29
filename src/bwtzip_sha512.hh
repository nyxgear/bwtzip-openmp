// bwtzip_sha512.hh - bwtzip SHA-512 functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_SHA512_HH
#define BWTZIP_SHA512_HH

#include <vector>

namespace bwtzip {
    // Returns the 512-bit (64-byte) SHA-512 hash of v
    std::vector<unsigned long long> sha512(const std::vector<unsigned char>& v);
}

#endif // End of: #ifndef BWTZIP_SHA512_HH
