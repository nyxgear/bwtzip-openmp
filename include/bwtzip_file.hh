// bwtzip_file.hh - bwtzip File functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_FILE_HH
#define BWTZIP_FILE_HH

#include <vector>
#include <cstdio>
#include <string>

namespace bwtzip {
    std::vector<unsigned char> readFile(const char * filename);
    const std::vector<unsigned char>& writeFile(
        const std::vector<unsigned char>& v,
        const char * filename
    );

    class InputFile {
    public:
        InputFile(const char * filename);
        std::vector<unsigned char> extractAtMost(unsigned long maxsize);
        ~InputFile();
    private:
        FILE * fp;
    };

    class OutputFile {
    public:
        OutputFile(const char * filename);
        void append(const std::vector<unsigned char>& v);
        ~OutputFile();
    private:
        FILE * fp;
    };

    namespace file {
        long   fileSize(FILE * fp); // Does not change position of file
        void   fileClose(FILE * fp);
        FILE * fileOpenRB(const char * filename);
        FILE * fileOpenWB(const char * filename);
    }
}

#endif // End of: #ifndef BWTZIP_FILE_HH
