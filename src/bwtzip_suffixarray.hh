// bwtzip_suffixarray.hh - bwtzip Suffix Array functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_SUFFIXARRAY_HH
#define BWTZIP_SUFFIXARRAY_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& bwtSarray(std::vector<unsigned char>& v);

    namespace manbermyers {
        std::vector<size_t> computePos(const std::vector<short>& a);
    }
}

#endif // End of: #ifndef BWTZIP_SUFFIXARRAY_HH
