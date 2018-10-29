// bwtzip_ahuff.hh - bwtzip Adaptive Huffman Coding functions
// GPLed - Joergen Ibsen - http://www.ibsensoftware.com/

// Partially based on an implementation from:
//    The Data Compression Book, 2nd ed.
//    by Mark Nelson
//    M&T Books, 1996, ISBN: 1558514341

#ifndef BWTZIP_AHUFF_HH
#define BWTZIP_AHUFF_HH

#include "bwtzip_common.hh"
#include <vector>

namespace bwtzip {
    std::vector<unsigned char>& ahuff(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unahuff(std::vector<unsigned char>& v);

    namespace adaptivehuffman {
        const long ESCAPE                   = 257;
        const unsigned long NUM_SYMBOLS     = 258;

        const unsigned long MAX_FREQUENCY   = 16384;
        const unsigned long INCREMENT_VALUE = 1; // only 1 is supported atm

        class AHuffTree {
        public:
            AHuffTree();
            void add(const int sym);
            void update(const int sym);

            class DecodeIterator {
            public:
                DecodeIterator(const AHuffTree *tree)
                    : tree(tree), current(0) { }

                bool isDone() const { return tree->nodes[current].child_is_leaf; }
                void next(const bool bit) { current = tree->nodes[current].child + bit; }
                int getSymbol() const { return tree->nodes[current].child; }

            private:
                const AHuffTree *tree;
                int current;
            };

            class EncodeIterator {
            public:
                EncodeIterator(AHuffTree *tree, const int sym)
                    : tree(tree), was_escaped(false) {

                    current = tree->leafs[sym];
                    if (current == -1) {
                        current = tree->leafs[ESCAPE];
                        was_escaped = true;
                    }
                }

                bool isDone() const { return current == 0; }
                void next() { current = tree->nodes[current].parent; }
                bool getBit() const { return (current & 1) == 0; }
                bool escaped() const { return was_escaped; }

            private:
                const AHuffTree *tree;
                int current;
                bool was_escaped;
            };

        private:
            void rescale();
            void swap_nodes(size_t n1, size_t n2);

            struct Node {
                size_t weight;
                int parent;
                bool child_is_leaf;
                size_t child;
            };

            std::vector<Node> nodes;
            std::vector<int> leafs;
            int next_free_node;
        };

        class AHuffEncoder {
        public:
            void encodeSymbol(const int sym);
            std::vector<unsigned char> finalize();

        private:
            bwtzip::PackedBits output;
            AHuffTree aht;
        };

        class AHuffDecoder {
        public:
            AHuffDecoder(std::vector<unsigned char> *pv) : pvec(pv), loc(0) { }
            int decodeSymbol();

        private:
            std::vector<unsigned char> * pvec;
            size_t                       loc;
            AHuffTree                    aht;
            bool inputBit();
        };
    }
}

#endif // End of: #ifndef BWTZIP_AHUFF_HH
