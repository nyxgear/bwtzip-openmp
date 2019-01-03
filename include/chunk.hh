// chunk.hh - PNG Chunk Class
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef CHUNK_HH
#define CHUNK_HH

#include "ucell.hh"

class Chunk {
public:

    Chunk(unsigned long int thetype);
    Chunk(Ucell& pngstream);
    Chunk& setData(const Gcell<unsigned char>& g);
    Ucell getData() const;
    unsigned long int getType() const;
    Gcell<unsigned char> getFlatRep() const;
    unsigned long int getCRC() const;
    bool matchCRC(unsigned long int crc) const;
    ~Chunk();

private:
    unsigned long   type;
    Ucell           data;
};

#endif // End of: #ifndef CHUNK_HH
