// bwtunzip.hh - bwtunzip functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTUNZIP_HH
#define BWTUNZIP_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char> &bwtDecompress(std::vector<unsigned char> &v);

    std::vector<unsigned char> &unbwt(std::vector<unsigned char> &v);

    void bwtunzipMain(int argc, char *argv[]);
}

#endif // End of: #ifndef BWTUNZIP_HH
