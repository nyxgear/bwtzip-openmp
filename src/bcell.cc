// bcell.cc - Memory Cell Abstraction Interface (Bcell: Byte Mcell)
// Miscellaneous functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bcell.hh"

// Default constructor, creates an empty cell
Bcell::Bcell() : Mcell<unsigned char>() { /* GNDN */ }

// Construct a zero-filled cell of a given name and size.
Bcell::Bcell(string s, size_t n) : Mcell<unsigned char>(s, n) {
    fill(0);
}

// Copy constructor
Bcell::Bcell(const Mcell<unsigned char>& c) : Mcell<unsigned char>(c.getName(), c.getSize()) {
    *this = c;
}

Bcell::Bcell(const vector<unsigned char> &v) : Mcell<unsigned char>("vector<unsigned char>", v.size()) {
    *this = v;
}

// Print the contents of our cell to cout
void Bcell::print() const {
    cout << "Cell: " << name << " | Size: " << size << endl;
    for (size_t st = 0; st < size; st++) {
        int byte = (*this)[st];
        cout << st << ": " << byte << endl;
    }
}

// Make our cell represent ull big-endian.
Bcell& Bcell::operator=(unsigned long long int ull) {
    alloc("Unsigned long long int", 8);

    for (int i = 0; i < 8; i++) {
        (*this)[i] = decomposeULL(ull, i);
    }

    return *this;
}

// Make our cell represent ull big-endian.
Bcell::Bcell(unsigned long long int ull) : Mcell<unsigned char>() {
    *this = ull;
}

Bcell::Bcell(const Mcell<bool>& c) : Mcell<unsigned char>() {
    *this = c;
}

// Set the nth bit of our cell to b
void Bcell::setBit(size_t n, bool b) {
    unsigned char uc = b;
    (*this)[n / 8] |= uc << (7 - n % 8);
}

// Get the nth bit of our cell
bool Bcell::getBit(size_t n) const {
    return ((*this)[n / 8] >> (7 - n % 8)) & 1;
}

// RETURN: The unsigned long int encoded by 4 bytes of an
//         Bcell starting at location n.
// NOTE:   Big-endian.                                    
unsigned long int Bcell::readUL(size_t n) const {
    unsigned long int ul = 0;
    
    for (int i = 0; i < 4; i++) {
        unsigned long int t = (*this)[n + i];
        ul |= t << (24 - 8 * i);
    }

    return ul;
}

// RETURN: The unsigned long long int encoded by 8 bytes of an
//         Bcell starting at location n.
// NOTE:   Big-endian.                                    
unsigned long long int Bcell::readULL(size_t n) const {
    unsigned long long int ull = 0;
    
    for (int i = 0; i < 8; i++) {
        unsigned long long int t = (*this)[n + i];
        ull |= t << (56 - 8 * i);
    }

    return ull;
}

// PURPOSE: Make our cell into a copy of the given cell.
Bcell & Bcell::operator= (const Mcell<unsigned char>& c) {
    if (&c != this) {
        alloc(c.getName(), c.getSize());
        transcopy(size, 0, c, 0);
    }

    return *this;
}

Bcell& Bcell::operator=(const vector<unsigned char>& v) {
    alloc("vector<unsigned char>", v.size());
    for (size_t i = 0; i < v.size(); i++) {
        (*this)[i] = v[i];
    }
    return *this;
}

Bcell::operator vector<unsigned char>() const {
    vector<unsigned char> ret(size);
    for (size_t i = 0; i < size; i++) {
        ret[i] = (*this)[i];
    }
    return ret;
}

Bcell& Bcell::operator=(const list<unsigned char>& l) {
    alloc("STL List", l.size());

    list<unsigned char>::const_iterator it = l.begin();
    for (size_t i = 0; i < size; i++) {
        (*this)[i] = *it++;
    }
    return *this;
}

Bcell& Bcell::operator= (const Mcell<bool>& c) {
    alloc(c.getName(), bitsToBytes(c.getSize()));
    fill(0);
    for (size_t i = 0; i < c.getSize(); i++) {
        setBit(i, c[i]);
    }
    return *this;
}

// Return an Mcell<size_t> of length 256 with the byte frequencies of our cell.
Mcell<size_t> Bcell::getFrequencies() const {
    Mcell<size_t> freq("Frequencies", 256);

    freq.fill(0);

    for (size_t st = 0; st < size; st++) {
        freq[(*this)[st]]++;
    }

    return freq;
}

Gcell<unsigned char> Bcell::getGcell() const {
    Gcell<unsigned char> ret;

    for (size_t i = 0; i < size; i++) {
        ret.appendData((*this)[i]);
    }

    return ret;
}

// Destructor
Bcell::~Bcell() { /* GNDN */ }
