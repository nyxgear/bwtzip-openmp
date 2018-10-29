// bwtzip_srle.hh - bwtzip Switched RLE functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// Phil Norman's Switched-RLE algorithm (srle)
// The compressor and decompressor can be in one of two states,
// LITERAL and FILL, and they start in LITERAL. The state means
// what type of run is currently being described. Both types of
// runs are prefixed by a single header byte, which describes
// how long the run is. A LITERAL header is then followed by
// that many bytes in the clear. A FILL header stands alone
// and represents a number of identical bytes. The byte used in
// the fill is the last literal byte which was output.
// The states are not directly encoded in the output. Instead,
// the length of the runs signals what state the next run will
// be. If the previous header was not 255, then the next state
// will be different. If the previous header was 255, then
// the next state will stay the same.
// To encode a run of exactly 255 elements, one header of
// 255 followed by one header of 0 is used.

#ifndef BWTZIP_SRLE_HH
#define BWTZIP_SRLE_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& srle(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unsrle(std::vector<unsigned char>& v);
}

#endif // End of: #ifndef BWTZIP_SRLE_HH
