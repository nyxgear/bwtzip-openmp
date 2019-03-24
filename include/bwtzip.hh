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
#include "wclock.hh"
#include "wclock.hh"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>

namespace bwtzip {
    // stats
    extern double stats_functions_time[];
    extern int stats_was_slowest_count[];
    extern double stats_time_averages[];

    /**
     * Update pipeline statics after iteration
     * @param iter
     */
    void stats_update(int iter);

    template<typename T>
    std::vector<unsigned char> &bwtCompress(std::vector<unsigned char> &v, T bwtfxn) {
        using namespace std;
        using namespace bwtzip;

        clockStart();
        bwtfxn(v);
        stats_functions_time[0] = clockReport();
        if (LOG_BWTZIP) cout << "BWT:         " << stats_functions_time[0] << endl;

        mtf2(v);
        stats_functions_time[1] = clockReport();
        if (LOG_BWTZIP) cout << "MTF-2:       " << stats_functions_time[1] << endl;

        zleWheeler(v);
        stats_functions_time[2] = clockReport();
        if (LOG_BWTZIP) cout << "Wheeler ZLE: " << stats_functions_time[2] << endl;

        arith(v);
        stats_functions_time[3] = clockReport();
        if (LOG_BWTZIP) cout << "Arith:       " << stats_functions_time[3] << endl;

        return v;
    }

    template<typename T>
    void bwtzipMain(int argc, char *argv[], std::string executableName, std::string algorithmName, T bwtfxn) {
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
        if (LOG_BWTZIP) {
            cout << "BWT Algorithm: " << algorithmName << endl;
            cout << "Chunk Size: " << max_chunk_size << endl;
        }

        // remove file output if present
        remove(argv[argc - 1]);

        InputFile infile(argv[argc - 2]);
        OutputFile outfile(argv[argc - 1]);
        outfile.append(encodeULL(BWTZIP_SIG));

        if (LOG_STATISTICS_CSV) {
            string filename = argv[argc - 1];
            Log::bwtzip::csv::start_new_line(filename, max_chunk_size);
        }

        auto clk = new wClock();

        int iter = 0;
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
                if (LOG_BWTZIP) cout << "Cycles elapsed: " << rdtsc() - t << endl;
            }

            outfile.append(encodeUL(v.size()));
            outfile.append(id);
            outfile.append(v);

            if (LOG_STATISTICS || LOG_STATISTICS_CSV) stats_update(iter);
            iter++;
        }

        Log::bwtzip::stats_print_summary(--iter, stats_was_slowest_count, stats_time_averages);

        Log::bwtzip::print_total_exec_time(clk->report());

        if (LOG_STATISTICS_CSV) Log::bwtzip::csv::print_statistics_line();
        delete clk;
    }

    std::vector<size_t> convertPosToLengths(const std::vector<size_t> &pos);
}

#endif // End of: #ifndef BWTZIP_HH
