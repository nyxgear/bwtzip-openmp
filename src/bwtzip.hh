// bwtzip.hh - bwtzip functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_HH
#define BWTZIP_HH

#include "bwtzip_common.hh"
#include "bwtzip_mtf.hh"
#include "bwtzip_zle.hh"
#include "bwtzip_arith.hh"
#include "bwtzip_file.hh"
#include "bwtzip_common.hh"
#include "clock.hh"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>

namespace bwtzip {
    template <typename T> std::vector<unsigned char>& bwtCompress(std::vector<unsigned char>& v, T bwtfxn) {
        using namespace std;
        using namespace bwtzip;

        clockStart();
        bwtfxn(v);     cout << "BWT:         " << clockReport() << endl;
        mtf2(v);       cout << "MTF-2:       " << clockReport() << endl;
        zleWheeler(v); cout << "Wheeler ZLE: " << clockReport() << endl;
        arith(v);      cout << "Arith:       " << clockReport() << endl;

        return v;
    }

    template <typename T> void bwtzipMain(int argc, char *argv[], std::string executableName, std::string algorithmName, T bwtfxn) {
        using namespace std;
        using namespace bwtzip;

        if (argc < 3 || argc > 4) {
            cout << "USAGE: " << executableName << " infile outfile" << endl;
            cout << "USAGE: " << executableName << " chunksize infile outfile" << endl;
            exit(EXIT_SUCCESS);
        }
    
        const unsigned long max_chunk_size = (argc == 3) ? MAX_CHUNK_SIZE : atol(argv[1]);
    
        if (max_chunk_size < MIN_LENGTH_TO_COMPRESS) {
            cout << "Chunk size too small!" << endl;
            exit(EXIT_FAILURE);
        }
    
        cout << "BWT Algorithm: " << algorithmName << endl;
        cout << "Chunk Size: " << max_chunk_size << endl;
    
        InputFile infile(argv[argc - 2]);
        OutputFile outfile(argv[argc - 1]);
        outfile.append(encodeULL(BWTZIP_SIG));
    
        while (true) {
            vector<unsigned char> v = infile.extractAtMost(max_chunk_size);
            if (v.empty()) {
                break;
            }
    
            vector<unsigned char> id;
    
            if (v.size() < MIN_LENGTH_TO_COMPRESS) {
                id.push_back(0);
            } else {
                id.push_back(1);
                unsigned long long t = rdtsc();
                bwtCompress(v, bwtfxn);
                cout << "Cycles elapsed: " << rdtsc() - t << endl;
            }
    
            outfile.append(encodeUL(v.size()));
            outfile.append(id);
            outfile.append(v);
        }
    }

    std::vector<size_t> convertPosToLengths(const std::vector<size_t>& pos);
}

#endif // End of: #ifndef BWTZIP_HH
