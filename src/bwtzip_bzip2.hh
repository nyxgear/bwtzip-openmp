// bwtzip_bzip2.hh - bwtzip bzip2 functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_BZIP2_HH
#define BWTZIP_BZIP2_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& bzip2(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unbzip2(std::vector<unsigned char>& v, size_t originalsize);
}

#endif // End of: #ifndef BWTZIP_BZIP2_HH
