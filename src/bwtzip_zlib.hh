// bwtzip_zlib.hh - bwtzip zlib functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_ZLIB_HH
#define BWTZIP_ZLIB_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& compress(std::vector<unsigned char>& v);
    std::vector<unsigned char>& decompress(std::vector<unsigned char>& v, size_t originalsize);
}

#endif // End of: #ifndef BWTZIP_ZLIB_HH
