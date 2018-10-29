// bwtzip_huff.cc - bwtzip Huffman functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_common.hh"
#include "bwtzip_huff.hh"
#include <algorithm>
#include <deque>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::huffman;

vector<unsigned char>& bwtzip::huff(vector<unsigned char>& v) {
    const vector<size_t>        freqs       = getFrequencies(v);
    const vector<unsigned char> codelengths = huffmanCodeLengths(freqs);
    const vector<unsigned char> codes       = huffmanCodes(codelengths);

    PackedBits encoded;

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char byte = v[i];
        const unsigned char thecode = codes[byte];
        unsigned char bitsleft = codelengths[byte];

        while (bitsleft > 8) {
            encoded.push_back(0);
            --bitsleft;
        }

        for (unsigned char j = bitsleft; j > 0; j--) {
            encoded.push_back((thecode >> (j - 1)) & 1);
        }
    }

    v = codelengths + encoded.get_vuc();
    return v;
}

vector<unsigned char>& bwtzip::puff(vector<unsigned char>& v) {
    const vector<unsigned char> codelengths(v.begin(), v.begin() + 256);
    const vector<unsigned char> codes = huffmanCodes(codelengths);

    v.erase(v.begin(), v.begin() + 256);

    vector<unsigned char> decompressed;

    unsigned char mrs8b = 0;  // Most Recently Seen 8 Bits
    unsigned char nbs = 0;    // Number of Bits Seen

    for (size_t i = 0; i < v.size() * 8; i++) {
        ++nbs;                 // We're looking at another bit
        mrs8b <<= 1;           // Make space for the new bit
        mrs8b |= getBit(v, i); // Load in the new bit

        for (int j = 0; j < 256; j++) {
            if (codelengths[j] == nbs && codes[j] == mrs8b) {
                decompressed.push_back(j);
                mrs8b = 0;
                nbs = 0;
                break;
            }
        }
    }

    swap(v, decompressed);
    return v;
}

bool bwtzip::huffman::getBit(const vector<unsigned char>& v, size_t i) {
    return (v[i / 8] >> (7 - i % 8)) & 1;
}

vector<size_t> bwtzip::huffman::getFrequencies(const vector<unsigned char>& v) {
    vector<size_t> freq(256, 0);

    for (size_t i = 0; i < v.size(); i++) {
        freq[v[i]]++;
    }

    return freq;
}

vector<unsigned char>
    bwtzip::huffman::huffmanCodeLengths(const vector<size_t>& freq) {

    vector<Tree> nodes; // All nodes will be in this vector. They will
    nodes.reserve(511); // not move during their lifetime.

    for (int i = 0; i < 256; i++) {
        nodes.push_back(Tree(freq[i], i));
    }

    deque<Tree *> leaves;
    for (int i = 0; i < 256; i++) {
        leaves.push_back(&nodes[i]);
    }

    sort(leaves.begin(), leaves.end(), lowerFreqThan);
    // Now, the lowest-frequency leaf is at leaves.front()

    deque<Tree *> internals;

    for (int i = 256; i < 511; i++) {
        Tree * first = NULL;
        Tree * second = NULL;

        if (leaves.empty()) {
            first = internals.front(); internals.pop_front();
        } else if (internals.empty() || lowerFreqThan(leaves.front(), internals.front())) {
            first = leaves.front();    leaves.pop_front();
        } else {
            first = internals.front(); internals.pop_front();
        }

        if (leaves.empty()) {
            second = internals.front(); internals.pop_front();
        } else if (internals.empty() || lowerFreqThan(leaves.front(), internals.front())) {
            second = leaves.front();    leaves.pop_front();
        } else {
            second = internals.front(); internals.pop_front();
        }

        nodes.push_back(Tree(first, second));
        internals.push_back(&nodes[i]);
    }

    return nodes[510].readCodeLengths();
}

vector<unsigned char>
    bwtzip::huffman::huffmanCodes(const vector<unsigned char>& codelengths) {

    vector<unsigned char> codes(256);

    unsigned char start = 0;    // Calculate right-aligned start positions

    for (int i = 255; i >= 0; i--) {
        // Fill in codes with codelength i
        for (int j = 0; j < 256; j++) {
            if (codelengths[j] == i) {
                codes[j] = start++;
            }
        }

        start >>= 1;
    }

    return codes;
}

bool bwtzip::huffman::lowerFreqThan(const Tree * lhs, const Tree * rhs) {
    return *lhs < *rhs;
}
