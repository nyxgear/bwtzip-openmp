// bwtzip_suffixbentleysedgewick.hh - bwtzip Bentley-Sedgewick Suffix Sort functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_SUFFIXBENTLEYSEDGEWICK_HH
#define BWTZIP_SUFFIXBENTLEYSEDGEWICK_HH

#include <vector>
#include <utility>

namespace bwtzip {
    std::vector<unsigned char>& bwtBentleySedgewick(std::vector<unsigned char>& v);

    namespace bentleysedgewick {
        int medianOfThree(int a, int b, int c);
        int findMedian(const std::vector<std::pair<const int *, const int *> >& v, size_t l, size_t r);
        void triPartition(std::vector<std::pair<const int *, const int *> >& v);
    }
}

#endif // End of: #ifndef BWTZIP_SUFFIXBENTLEYSEDGEWICK_HH
