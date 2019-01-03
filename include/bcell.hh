// bcell.hh - Memory Cell Abstraction Interface (Bcell: Byte Mcell)
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// The Bcell is derived from the Mcell<unsigned char> and overrides
// few of its methods. It stores no extra data. It simply
// provides extra methods which work on byte-oriented data.
// (The Bcell ruthlessly assumes that bytes are 8-bit.)

// Like Mcells, empty Bcells can be constructed, or given a name and
// a size upon construction. (Unlike Mcells, Bcells are zero-filled
// upon construction.) There is also a Bcell constructor that
// reads in a named file into memory. Finally, there is a Bcell copy
// constructor which allows Bcells and Mcell<unsigned char> to be
// used nearly interchangably. (As of yet there is no builtin method
// to downconvert a Bcell into an Mcell<unsigned char>, as a cast can
// do this easily.)

// With the print() method, the entire contents of a Bcell can be examined.

// Bcells support bitwise reading and writing in a big-endian fashion,
// though it is not quite as convenient as an Mcell<bool>.
// Bcells also support reading unsigned long (4 byte) and
// unsigned long long (8 byte) big-endian values.

// Bcells support many different types of compression. If ZIP is defined,
// the compress() and decompress(size_t originalsize) methods become
// available, which use zlib to compress the cell as much as possible.
// Also the bzip2() and unbzip2(unsigned int originalsize) methods become
// available, which use bzip2. Compression followed by decompression does
// not change the cell's contents at all.

// Bcells support Secure Hash Algorithm-1 one-way cryptographically secure
// hashing. The SHA1() method returns a 20-byte Bcell containing the SHA-1
// hash of our cell. SHA-512 has not yet been implemented in Bcell, but it
// easily could be.

// Bcells support easy file access. In addition to the file-reading
// constructor, the readFile(FILE *fp) method fills the cell with the
// entire contents of a file, and writeFile(FILE *fp) dumps the cell's
// contents to a file. writeFileName(const char *fname) does the same
// with a named file.

// Bcells support assignment, too. operator= which takes a
// const Mcell<unsigned char>& is provided so that Bcells work with all
// the usual Mcell operations. (+ and += return Mcells.) Also, a
// Bcell can be set with operator= to an unsigned long long int,
// which makes the Bcell hold 8 bytes which represent the unsigned
// long long int. (There does not yet exist a constructor for this, but
// writing one would be trivial.)
// NOTE: TAKE CARE TO CAST A VALUE TO UNSIGNED LONG LONG BEFORE ASSIGNING
// IT TO A BCELL. Eventually a method that works with unsigned longs, etc.
// may be provided.

// Bcells support frequency statistics: getFrequencies() returns an
// Mcell<size_t> whose Nth entry contains how many times byte N occurs
// in our cell.

// Finally, Bcells support the home-grown compression of bwtzip.
// bwtCompressStree() and bwtDecompressStree() use the suffix tree algorithm
// combined with several other improvements to compress our cell. Also
// provided are bwtCompressSarray() and bwtDecompressSarray() which use the
// suffix array algorithm.
// NOTE: THEY ARE NOT INTERCHANGABLE: IF YOU COMPRESS WITH SUFFIX ARRAYS YOU
// MUST USE THE SUFFIX ARRAY DECOMPRESSOR, AND LIKEWISE WITH SUFFIX TREES.
// This is due to minor differences in the algorithms, which otherwise achieve
// identical compression. Suffix trees are faster and linear-time.

// Behind the scenes, bwtzip compression is implemented with a host of
// functions private to Bcell. These are still written in a modular, fully
// abstracted fashion.

#ifndef BCELL_HH
#define BCELL_HH

#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <vector>
#include "common.hh"
#include "mcell.hh"
#include "clock.hh"
#include "gcell.hh"
#include "ucell.hh"
#include "chunk.hh"
#include "trio.hh"
#include "bwtzip_mtf.hh"
#include "bwtzip_zle.hh"
#include "bwtzip_arith.hh"
#include "bwtzip_common.hh"
#include "bwtzip_suffixtree.hh"
#include "bwtzip_file.hh"

using namespace std;
using namespace bwtzip;

class Bcell : public Mcell<unsigned char> {

public:

// CONSTRUCTORS

    Bcell();                                    // Default constructor, creates an empty cell
    Bcell(string s, size_t n);                  // Construct a zero-filled cell
                                                // of a given name and size.
    Bcell(const Mcell<unsigned char>& c);       // Copy constructor
    explicit Bcell(unsigned long long int ull); // Make our cell represent ull big-endian.
    Bcell(const Mcell<bool>& c);                // Copy constructor
    Bcell(const vector<unsigned char> &v);

// ACCESSORS, MUTATORS

    void setBit(size_t n, bool b);          // Set the nth bit of our cell to b

    bool getBit(size_t n) const;            // Get the nth bit of our cell

    unsigned long int                       // Return the unsigned long int
        readUL(size_t n) const;             // encoded big-endian by 4 bytes
                                            // starting at location n.
    unsigned long long int
        readULL(size_t n) const;

    void print() const;                     // Print the contents of our cell to cout

// PNG

    unsigned long getCRC() const;

    Bcell& pngacc();

    Bcell& refilter(unsigned long width, unsigned long height,
        unsigned char bitdepth, unsigned char colortype, unsigned int filter);

// ASSIGNMENT

    Bcell& operator=                        // Make our cell represent ull big-endian.
        (unsigned long long int ull);

    Bcell& operator=(const Mcell<bool>& c);

    Bcell& operator=(const Mcell<unsigned char>& c);
    Bcell& operator=(const list<unsigned char>& l);
    Bcell& operator=(const vector<unsigned char>& v);
    operator vector<unsigned char>() const;

// ETC

    Mcell<size_t> getFrequencies() const;   // Return an Mcell<size_t> of
                                            // length 256 with the byte
                                            // frequencies of our cell.

    Gcell<unsigned char> getGcell() const;

// DESTRUCTOR

    virtual ~Bcell();                       // Destructor

private:

// PNG

    static unsigned char samplesPerPixel(unsigned char colortype);

    static unsigned long bytesPerUnfilteredScanLine(
        unsigned long width, unsigned char bitdepth, unsigned char colortype);

    static unsigned long bytesPerFilteredScanLine(
        unsigned long width, unsigned char bitdepth, unsigned char colortype);

    static unsigned long decompressedSize(
        unsigned long width, unsigned long height,
        unsigned char bitdepth, unsigned char colortype);

    static void checkBitdepthAndColortype(
        unsigned char bitdepth, unsigned char colortype);

    static unsigned char paethPredictor(
        unsigned char a, unsigned char b, unsigned char c);

    static unsigned char bpp(
        unsigned char bitdepth, unsigned char colortype);
};

#endif // End of: #ifndef BCELL_HH
