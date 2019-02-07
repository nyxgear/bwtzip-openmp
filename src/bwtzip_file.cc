// bwtzip_file.cc - bwtzip File functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_file.hh"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::file;

vector<unsigned char> bwtzip::readFile(const char * filename) {
    FILE * fp = fileOpenRB(filename);
    vector<unsigned char> ret(fileSize(fp));

    if (fread(&ret[0], 1, ret.size(), fp) != ret.size() || ferror(fp)) {
        throw string("An error occurred while reading from \"") + filename + "\".";
    } else {
        fileClose(fp);
        return ret;
    }
}

const vector<unsigned char>& bwtzip::writeFile(
    const vector<unsigned char>& v, const char * filename) {

    FILE * fp = fileOpenWB(filename);

    if (fwrite(&v[0], 1, v.size(), fp) != v.size() || ferror(fp)) {
        throw string("An error occurred while writing to \"") + filename + "\".";
    } else {
        fileClose(fp);
        return v;
    }
}

bwtzip::InputFile::InputFile(const char * filename) : fp(fileOpenRB(filename)) { }

vector<unsigned char> bwtzip::InputFile::extractAtMost(unsigned long maxsize) {
    vector<unsigned char> ret;
    ret.reserve(maxsize);

    while (ret.size() < maxsize) {
        int c = getc(fp);
        if (c != EOF) {
            ret.push_back(c);
        } else {
            break;
        }
    }

    return ret;
}

bwtzip::InputFile::~InputFile() {
    try {
        fileClose(fp);
    } catch (string e) {
        cout << "An error occurred while closing an InputFile." << endl;
        exit(EXIT_FAILURE);
    }
}

bwtzip::OutputFile::OutputFile(const char * filename) : fp(fileOpenWB(filename)) { }

void bwtzip::OutputFile::append(const vector<unsigned char>& v) {
    for (size_t i = 0; i < v.size(); i++) {
        putc(v[i], fp);
    }
}

bwtzip::OutputFile::~OutputFile() {
    try {
        fileClose(fp);
    } catch (string e) {
        cout << "An error occurred while closing an OutputFile." << endl;
        exit(EXIT_FAILURE);
    }
}

long bwtzip::file::fileSize(FILE * fp) {
    const long orig = ftell(fp);
    fseek(fp, 0, SEEK_END);
    const long size = ftell(fp);
    fseek(fp, orig, SEEK_SET);
    return size;
}

void bwtzip::file::fileClose(FILE * fp) {
    if (fclose(fp) == EOF) {
        throw string("An error occurred while closing a file.");
    }
}

FILE * bwtzip::file::fileOpenRB(const char * filename) {
    FILE * fp = fopen(filename, "rb");

    if (fp != NULL) {
        return fp;
    } else {
        throw string("An error occurred while opening \"")
            + filename + "\" for input.";
    }
}

FILE * bwtzip::file::fileOpenWB(const char * filename) {
    FILE * fp = fopen(filename, "rb");

    if (fp != NULL) {
        fileClose(fp);
        throw string("The output file \"") + filename + "\" already exists.";
    }

    fp = fopen(filename, "wb");

    if (fp != NULL) {
        return fp;
    } else {
        throw string("An error occurred while opening \"")
            + filename + "\" for output.";
    }
}
