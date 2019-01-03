// bwtzip_sha1.hh - bwtzip SHA-1 functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_SHA1_HH
#define BWTZIP_SHA1_HH

#include <vector>

namespace bwtzip {
    // Returns the 160-bit (20-byte) SHA-1 hash of v
    std::vector<unsigned char> sha1(const std::vector<unsigned char>& v);
}

#endif // End of: #ifndef BWTZIP_SHA1_HH
