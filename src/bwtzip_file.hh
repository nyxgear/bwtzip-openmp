// bwtzip_file.hh - bwtzip File functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_FILE_HH
#define BWTZIP_FILE_HH

#include <vector>
#include <cstdio>
#include <string>

namespace bwtzip {
    std::vector<unsigned char> readFile(const char * filename) throw (std::string);
    const std::vector<unsigned char>& writeFile(
        const std::vector<unsigned char>& v,
        const char * filename
    ) throw (std::string);

    class InputFile {
    public:
        InputFile(const char * filename) throw (std::string);
        std::vector<unsigned char> extractAtMost(unsigned long maxsize) throw ();
        ~InputFile() throw ();
    private:
        FILE * fp;
    };

    class OutputFile {
    public:
        OutputFile(const char * filename) throw (std::string);
        void append(const std::vector<unsigned char>& v) throw ();
        ~OutputFile() throw ();
    private:
        FILE * fp;
    };

    namespace file {
        long   fileSize(FILE * fp); // Does not change position of file
        void   fileClose(FILE * fp) throw (std::string);
        FILE * fileOpenRB(const char * filename) throw (std::string);
        FILE * fileOpenWB(const char * filename) throw (std::string);
    }
}

#endif // End of: #ifndef BWTZIP_FILE_HH
