// bwtzip_srle.cc - bwtzip Switched RLE functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_srle.hh"
#include "bwtzip_common.hh"
using namespace std;

vector<unsigned char>& bwtzip::srle(vector<unsigned char>& v) {
    vector<unsigned char> output;
    vector<unsigned char> buf;
    bool literal  = true;
    size_t count = 0;
    size_t n = 0;
    unsigned char fillbyte = 0;

    while (n < v.size()) {
        // We are looking at the start of a new run
        if (literal) { // LITERAL
            while (n < v.size() && count < 255) {
                fillbyte = v[n];
                buf.push_back(v[n]);
                n++;
                count++;

                if (n != v.size() && v[n - 1] == v[n]) {
                    break;
                }
            }
        } else { // FILL
            // We are looking for up to 255 identical bytes.
            while (n < v.size() && count < 255) {
                if (v[n] == fillbyte) {
                    n++;
                    count++;
                } else {
                    break;
                }
            }
        }

        // Run is over. Header byte must be count.

        output.push_back(count);

        if (literal) { // LITERAL
            output += buf;
            buf.clear();
        }

        if (count != 255) {
            literal = !literal;
        }

        count = 0;
    }

    swap(v, output);
    return v;
}

vector<unsigned char>& bwtzip::unsrle(vector<unsigned char>& v) {
    vector<unsigned char> output;
    bool literal = true;
    size_t n = 0;
    unsigned char fillbyte = 0;

    while (n < v.size()) {
        unsigned char header = v[n++];

        if (literal) { // LITERAL
            for (int i = 0; i < header; i++) {
                fillbyte = v[n++];
                output.push_back(fillbyte);
            }
        } else { // FILL
            for (int i = 0; i < header; i++) {
                output.push_back(fillbyte);
            }
        }

        if (header != 255) {
            literal = !literal;
        }
    }

    swap(v, output);
    return v;
}
