// bcell_png.cc - Memory Cell Abstraction Interface (Bcell: Byte Mcell)
// PNG functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bcell.hh"
#include "bwtzip_zlib.hh"
using namespace bwtzip;

// Return the CRC of our cell
unsigned long int Bcell::getCRC() const {
    unsigned long int c = 0xFFFFFFFFUL;

    for (size_t i = 0; i < size; i++) {
        c = CRC_TABLE[c & 0xFF ^ (*this)[i]] ^ (c >> 8);
    }

    return c ^ 0xFFFFFFFFUL;
}

Bcell& Bcell::pngacc() {
    Ucell file = getGcell();

    if (file.consumeULL() != PNG_SIG) {
        cout << "ERROR: In Bcell::pngacc(), signature NOT recognized!" << endl;
        exit(EXIT_FAILURE);
    }

    Ucell flat(PNG_SIG);

    Chunk ihdr(file);

    Ucell ihdrdata = ihdr.getData();
    
    unsigned long width = ihdrdata.consumeUL();
    unsigned long height = ihdrdata.consumeUL();
    unsigned char bitdepth = ihdrdata.popFrontData();
    unsigned char colortype = ihdrdata.popFrontData();

    ihdrdata.popFrontData();
    ihdrdata.popFrontData();

    unsigned char interlacemethod = ihdrdata.popFrontData();

    if (interlacemethod != 0) {
        cout << "ERROR: In Bcell::pngacc(), I won't work on interlaced images." << endl;
        exit(EXIT_FAILURE);
    }

    flat += ihdr.getFlatRep();

    Ucell idatcontents;
    bool seenidat = false;

    while (file.getSize() != 0) {
        Chunk chunk(file);

        if (chunk.getType() != PNG_IDAT && seenidat) {
            // End of IDAT chunks
            seenidat = false;

            Bcell cell = idatcontents.getMcell();
            idatcontents.clear();

            vector<unsigned char> v = cell;
            decompress(v, decompressedSize(width, height, bitdepth, colortype));
            cell = v;
            cell.refilter(width, height, bitdepth, colortype, 0);
            v = cell;
            compress(v);
            cell = v;

            for (size_t i = 0; i < cell.getSize(); i++) {
                idatcontents.appendData(cell[i]);
            }

            Chunk synthidat(PNG_IDAT);
            synthidat.setData(idatcontents);
            flat += synthidat.getFlatRep();
        }

        if (chunk.getType() == PNG_gAMA
        || chunk.getType() == PNG_PLTE
        || chunk.getType() == PNG_tRNS
        || chunk.getType() == PNG_IEND) {

            flat += chunk.getFlatRep();
        } else if (chunk.getType() == PNG_IDAT) {
            seenidat = true;
            idatcontents += chunk.getData();
        } else {
            // GNDN
        }
    }

    *this = flat.getMcell();
    return *this;
}

Bcell& Bcell::refilter(unsigned long width, unsigned long height,
    unsigned char bitdepth, unsigned char colortype, unsigned int filter) {
    
    if (filter > 0) {
        cout << "ERROR: Invalid filter passed to Bcell::refilter()" << endl;
        exit(EXIT_FAILURE);
    }
    
    Mcell<unsigned char> filtercodes("Filter Codes", height);
    Mcell<Mcell<unsigned char> > scanlines("Scanlines", height);

    const unsigned long bytewidth = bytesPerUnfilteredScanLine(width, bitdepth, colortype);

    for (size_t i = 0; i < height; i++) {
        scanlines[i].alloc("Scanline", bytewidth);
    }

    size_t n = 0;

    for (size_t i = 0; i < height; i++) {
        filtercodes[i] = (*this)[n++];
        for (size_t j = 0; j < bytewidth; j++) {
            scanlines[i][j] = (*this)[n++];
        }
    }

    // Do unfiltering, refiltering work here

    n = 0;
    for (size_t i = 0; i < height; i++) {
        (*this)[n++] = filtercodes[i];
        for (size_t j = 0; j < bytewidth; j++) {
            (*this)[n++] = scanlines[i][j];
        }
    }    

    return *this;
}

// STATICS

unsigned char Bcell::samplesPerPixel(unsigned char colortype) {
    switch (colortype) {
    case 0:
        return 1;
    case 2:
        return 3;
    case 3:
        return 1;
    case 4:
        return 2;
    case 6:
        return 4;
    default:
        cout << "ERROR: Bcell::samplesPerPixel() given unrecognized color type!" << endl;
        exit(EXIT_FAILURE);
    }
}

unsigned long Bcell::bytesPerUnfilteredScanLine(
    unsigned long width, unsigned char bitdepth, unsigned char colortype) {

    checkBitdepthAndColortype(bitdepth, colortype);
    return bitsToBytes(bitdepth * samplesPerPixel(colortype) * width);
}

unsigned long Bcell::bytesPerFilteredScanLine(
    unsigned long width, unsigned char bitdepth, unsigned char colortype) {

    checkBitdepthAndColortype(bitdepth, colortype);
    return bytesPerUnfilteredScanLine(width, bitdepth, colortype) + 1;
}

unsigned long Bcell::decompressedSize(
    unsigned long width, unsigned long height,
    unsigned char bitdepth, unsigned char colortype) {

    checkBitdepthAndColortype(bitdepth, colortype);
    return bytesPerFilteredScanLine(width, bitdepth, colortype) * height;
}

unsigned char Bcell::bpp(unsigned char bitdepth, unsigned char colortype) {
    checkBitdepthAndColortype(bitdepth, colortype);

    return bitsToBytes(bitdepth * samplesPerPixel(colortype));
}

void Bcell::checkBitdepthAndColortype(unsigned char bitdepth, unsigned char colortype) {
    switch (colortype) {
    case 0:
        if (bitdepth == 1 || bitdepth == 2 || bitdepth == 4 
         || bitdepth == 8 || bitdepth == 16) {
            return;
        }

        break;
    case 2: case 4: case 6:
        if (bitdepth == 8 || bitdepth == 16) {
            return;
        }
        break;
    case 3:
        if (bitdepth == 1 || bitdepth == 2 || bitdepth == 4 
         || bitdepth == 8) {
            return;
        }
        break;
    default:
        cout << "ERROR: Bcell::checkBitdepthAndColortype() given unrecognized color type!" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "ERROR: Bcell::checkBitdepthAndColortype() given bad input!" << endl;
    exit(EXIT_FAILURE);
}

unsigned char Bcell::paethPredictor(unsigned char a, unsigned char b, unsigned char c) {
    int pa = (int) b - (int) c;
    int pb = (int) a - (int) c;
    int pc = pa + pb;

    if (pa < 0) pa *= -1;
    if (pb < 0) pb *= -1;
    if (pc < 0) pc *= -1;

    if (pa <= pb && pa <= pc)
        return a;
    else if (pb <= pc)
        return b;
    else
        return c;
}
