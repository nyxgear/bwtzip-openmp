// chunk.cc - PNG Chunk Class
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "chunk.hh"

Chunk::Chunk(unsigned long int thetype) : type(thetype) { }

Chunk::Chunk(Ucell& pngstream) {
    if (pngstream.getSize() < 12) {
        cout << "ERROR: In Chunk::Chunk(Ucell& pngstream)" << endl;
        cout << "       Given an pngstream with less than 12 bytes!" << endl;
        exit(EXIT_FAILURE);
    }

    const unsigned long length = pngstream.consumeUL();
    type = pngstream.consumeUL();

    if (pngstream.getSize() < length + 4) {
        cout << "ERROR: In Chunk::Chunk(Ucell& pngstream)" << endl;
        cout << "       Given an pngstream with insufficient length!" << endl;
        exit(EXIT_FAILURE);
    }

    setData(pngstream.popFront(length));

    const unsigned long int crc = pngstream.consumeUL();
    if (!matchCRC(crc)) {
        cout << "ERROR: In Chunk::Chunk(Ucell& pngstream)" << endl;
        cout << "       CRCs do not match!" << endl;
        exit(EXIT_FAILURE);
    }
}

Chunk& Chunk::setData(const Gcell<unsigned char>& g) {
    data = g;
    return *this;
}

Ucell Chunk::getData() const {
    Ucell ret = data;
    return ret;
}

unsigned long int Chunk::getType() const {
    return type;
}

Gcell<unsigned char> Chunk::getFlatRep() const {
    // A PNG chunk is represented as:
    // Length, Type, Data, CRC

    Gcell<unsigned char> ret;
    ret += Ucell((unsigned long) data.getSize());
    ret += Ucell(type);
    ret += data;
    ret += Ucell(getCRC());

    return ret;
}

bool Chunk::matchCRC(unsigned long int crc) const {
    return getCRC() == crc;
}

unsigned long int Chunk::getCRC() const {
    Ucell temp(type);
    temp += data;
    return temp.getCRC();
}

Chunk::~Chunk() { /* GNDN */ }
