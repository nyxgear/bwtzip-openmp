// bwtzip_mtf.hh - bwtzip Move-To-Front functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// The Move-To-Front (MTF) algorithm is a byte-for-byte streaming recoding.
// Conceptually, it takes an input byte and outputs the number of distinct
// other bytes which have been seen since its last occurrence. At the
// beginning, byte 0x00 is assumed to have most recently occurred, with
// 0x01 second most recent, etc.
// The input alphabet to MTF is [0, 255] and the output alphabet is [0, 255].

// In practice, a list of 256 bytes is maintained. In position 0 of this
// list, the most recently seen byte is stored. In position 1, the second
// most recently seen, etc. When a byte is read from the input stream, its
// location is found in the list and that location is emitted. Then
// that byte is moved to the front of the list.
// (If the byte read was the most recently seen byte, then a 0 is
// emitted and the list is not changed.)
// The reverse transformation follows a similar process (it can easily
// be understood by looking at the code).

// The MTF-1 algorithm changes how things are moved in the list. If the
// byte was at any position > 1, then it is moved to position 1. If the
// byte was at position 1, then it is moved to position 0. Otherwise it
// is not moved.

// The MTF-2 algorithm further changes how things are moved in the list.
// If the byte was at any position > 1, then it is moved to position 1.
// If the byte was at position 1 and the last output byte was not 0,
// then it is moved to position 0. Otherwise it is not moved.

#ifndef BWTZIP_MTF_HH
#define BWTZIP_MTF_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& mtf(std::vector<unsigned char>& v);
    std::vector<unsigned char>& mtf1(std::vector<unsigned char>& v);
    std::vector<unsigned char>& mtf2(std::vector<unsigned char>& v);
    std::vector<unsigned char>& ts0(std::vector<unsigned char>& v);

    std::vector<unsigned char>& unmtf(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unmtf1(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unmtf2(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unts0(std::vector<unsigned char>& v);
}

#endif // End of: #ifndef BWTZIP_MTF_HH
