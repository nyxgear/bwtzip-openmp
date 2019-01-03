// bwtzip_suffixstlsort.hh - bwtzip Suffix STL Sort functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_SUFFIXSTLSORT_HH
#define BWTZIP_SUFFIXSTLSORT_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& bwtSuffixStlSort(std::vector<unsigned char>& v);

    namespace stlsort {
        std::vector<size_t> computePos(const std::vector<short>& v);

        class Comparator {
        public:
            Comparator(const short * p) : p(p) { }
            bool operator()(const size_t& lhs, const size_t& rhs) {
                if (lhs == rhs) {
                    return false;
                }

                size_t l = lhs;
                size_t r = rhs;

                while (true) {
                    if (p[l] < p[r]) {
                        return true;
                    } else if (p[l] > p[r]) {
                        return false;
                    } else {
                        ++l;
                        ++r;
                    }
                }
            }
        private:
            const short * const p;
        };
    }
}

#endif // End of: #ifndef BWTZIP_SUFFIXSTLSORT_HH
