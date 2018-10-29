// ucell.cc - Memory Cell Abstraction Interface (Ucell: Unsigned Char Gcell)
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "ucell.hh"

Ucell::Ucell() : Gcell<unsigned char>() { /* GNDN */ }

Ucell::Ucell(const Gcell<unsigned char>& g) : Gcell<unsigned char>() {
    *this = g;
}

Ucell::Ucell(const char *fname) : Gcell<unsigned char>() {
    vector<unsigned char> v = readFile(fname);

    for (size_t i = 0; i < v.size(); i++) {
        appendData(v[i]);
    }
}

Ucell::Ucell(unsigned long ul) : Gcell<unsigned char>() {
    for (int i = 0; i < 4; i++) {
        appendData(decomposeUL(ul, i));
    }
}

Ucell::Ucell(unsigned long long ull) : Gcell<unsigned char>() {
    for (int i = 0; i < 8; i++) {
        appendData(decomposeULL(ull, i));
    }
}

unsigned long int Ucell::consumeUL() {
    if (size < 4) {
        cout << "ERROR: consumeUL() given a Ucell of size less than 4." << endl;
        exit(EXIT_FAILURE);
    }

    unsigned long int ul = 0;
    
    for (int i = 0; i < 4; i++) {
        unsigned long int t = popFrontData();
        ul |= t << (24 - 8 * i);
    }

    return ul;
}

unsigned long long int Ucell::consumeULL() {
    if (size < 8) {
        cout << "ERROR: consumeULL() given a Ucell of size less than 8." << endl;
        exit(EXIT_FAILURE);
    }

    unsigned long long int ull = consumeUL();
    ull <<= 32;
    ull |= consumeUL();
    
    return ull;
}

unsigned long int Ucell::readUL() const {
    if (size < 4) {
        cout << "ERROR: readUL() given a Ucell of size less than 4." << endl;
        exit(EXIT_FAILURE);
    }

    unsigned long int ul = 0;
    Node *temp = head;
    
    for (int i = 0; i < 4; i++) {
        unsigned long int t = temp->data;
        ul |= t << (24 - 8 * i);
        temp = temp->next;
    }

    return ul;
}

unsigned long int Ucell::getCRC() const {
    unsigned long int c = 0xFFFFFFFFUL;
    Node *temp = head;

    while (temp != NULL) {
        c = CRC_TABLE[c & 0xFF ^ temp->data] ^ (c >> 8);
        temp = temp->next;
    }

    return c ^ 0xFFFFFFFFUL;
}

Ucell& Ucell::operator=(const Gcell<unsigned char>& g) {
    if (this != &g) {
        clear();
        Gcell<unsigned char> temp = g;
        while (temp.getSize() != 0) {
            appendData(temp.popFrontData());
        }
    }
    return *this;
}

const Ucell& Ucell::writeFileName(const char *fname) const {
    vector<unsigned char> v;

    Node *temp = head;
    while (temp != NULL) {
        v.push_back(temp->data);
        temp = temp->next;
    }

    writeFile(v, fname);
    return *this;
}

Ucell::~Ucell() { /* GNDN */ }
