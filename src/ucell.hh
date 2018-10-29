// ucell.hh - Memory Cell Abstraction Interface (Ucell: Unsigned Char Gcell)
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// Ucell is derived from Gcell<unsigned char>. It stores no
// extra data and provides new methods only, which work on
// byte-oriented data. (It ruthlessly assumes that
// bytes are 8-bit.)

#ifndef UCELL_HH
#define UCELL_HH

#include <iostream>
#include "bwtzip_file.hh"
#include "gcell.hh"
#include "common.hh"
#include "bwtzip_common.hh"
#include <vector>
using namespace std;
using namespace bwtzip;

class Ucell : public Gcell<unsigned char> {
public:

    Ucell();
    Ucell(const Gcell<unsigned char>& g);
    Ucell(const char *fname);
    explicit Ucell(unsigned long ul);
    explicit Ucell(unsigned long long ull);
    unsigned long int       consumeUL();
    unsigned long long int  consumeULL();
    unsigned long int       readUL()        const;
    unsigned long int       getCRC()        const;
    Ucell& operator=(const Gcell<unsigned char>& g);
    const Ucell& writeFileName(const char *fname) const;
    virtual ~Ucell();
};

#endif // End of: #ifndef UCELL_HH
