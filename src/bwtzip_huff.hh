// bwtzip_huff.hh - bwtzip Huffman functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_HUFF_HH
#define BWTZIP_HUFF_HH

#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& huff(std::vector<unsigned char>& v);
    std::vector<unsigned char>& puff(std::vector<unsigned char>& v);

    namespace huffman {
        bool getBit(const std::vector<unsigned char>& v, size_t i);

        std::vector<size_t> getFrequencies(const std::vector<unsigned char>& v);

        // Given frequencies, return a vector of length 256 containing codelengths
        std::vector<unsigned char> huffmanCodeLengths(const std::vector<size_t>& freq);

        // Returns the lower 8 bits, right-aligned, of each code
        std::vector<unsigned char> huffmanCodes(const std::vector<unsigned char>& codelengths);

        class Tree {
        public:
            // Construct an empty tree
            Tree() : freq(0), byte(-1), left(NULL), right(NULL), up(NULL) { }

            // Fuse together two trees
            Tree(Tree* l, Tree* r)
                : freq(l->freq + r->freq), byte(-1), left(l), right(r), up(NULL) {

                l->up = r->up = this;
            }

            // Construct a one-node tree
            Tree(size_t f, short b)
                : freq(f), byte(b), left(NULL), right(NULL), up(NULL) { }

            // Return a vector of length 256 containing the codelengths of a tree
            std::vector<unsigned char> readCodeLengths() const {
                std::vector<unsigned char> ret(256);
                readCodeLengthsHelper(ret, 0);
                return ret;
            }

            bool operator<(const Tree& rhs) const {
                return freq < rhs.freq;
            }

        private:
            size_t  freq;
            short   byte;
            Tree *  left;
            Tree *  right;
            Tree *  up;

            void readCodeLengthsHelper(std::vector<unsigned char>& v, unsigned char h) const {
                if (left == NULL) { // We are at a leaf
                    v[byte] = h;
                } else {              // We are at an internal node
                    left->readCodeLengthsHelper(v, h + 1);
                    right->readCodeLengthsHelper(v, h + 1);
                }
            }
        };

        bool lowerFreqThan(const Tree * lhs, const Tree * rhs);
    }
}

#endif // End of: #ifndef BWTZIP_HUFF_HH
