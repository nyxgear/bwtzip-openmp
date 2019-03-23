#ifndef PBWTZIP_HH
#define PBWTZIP_HH

#include "bwtzip_common.hh"
#include "bwtzip_mtf.hh"
#include "bwtzip_zle.hh"
#include "bwtzip_arith.hh"
#include "bwtzip_file.hh"
#include "clock.hh"
#include "log.hh"
#include "wclock.hh"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>

#include <omp.h>
#include <math.h>
#include <queue>
#include <tuple>

#define NUM_THREAD_STAGE_1 4
#define NUM_THREAD_STAGE_2 1
#define NUM_THREAD_STAGE_3 1

#define MAX(a, b) a > b ? a : b
#define MAX3(a, b, c) (MAX(MAX((a), (b)), (c)))

// define the buffer size as the max of num thread assigned
#define BUFFER_SIZE MAX3(NUM_THREAD_STAGE_1, NUM_THREAD_STAGE_2, NUM_THREAD_STAGE_3)

// parallel bwtzip
namespace pbwtzip {
    using namespace std;
    using namespace bwtzip;

    typedef struct cnk_t {
        int id;
        vector<unsigned char> v;
    } cnk_t;

    // chunk buffer variables
    static cnk_t *bufferR_1[2][BUFFER_SIZE];
    static cnk_t *buffer1_2[2][BUFFER_SIZE];
    static cnk_t *buffer2_3[2][BUFFER_SIZE];
    static cnk_t *buffer3_W[2][BUFFER_SIZE];

    // turned true from read_file function once file read completed
    // (used to signal to write function that read is completed)
    static bool read_completed = false;
    // turned false from write_file function when, read completed and last
    // compressed chunk written to file
    // (used to signal file compression is complete)
    static bool ongoing_file_processing = true;

    // stats
    static double stats_stages_time[5];
    static int stats_lasted_longer_count[5];
    static double stats_time_averages[5];


    /**
     * Read file
     * @param bs                buffer side
     * @param infile            input file
     * @param max_chunk_size    maximum chunk size
     */
    void read_file(int bs, bwtzip::InputFile &infile, const unsigned long max_chunk_size);

    /**
     * Stage 1: bwt function applied to chunk
     * @param bs      buffer side
     * @tparam T      function return type
     * @param bwtfxn  bwt function
     */
    template<typename T>
    void stage_1(int bs, T bwtfxn) {
        Log::stage::started("stage_1", bs);
        auto clk = new wClock();

#pragma omp parallel for num_threads(NUM_THREAD_STAGE_1) firstprivate(bs)
        for (auto i = 0; i < BUFFER_SIZE; i++) {
            cnk_t *c;

            c = bufferR_1[bs][i];
            Log::stage::chunk_read("stage_1", "bufferR_1", bs, i, c);

            if (c != nullptr && c->id == 1) {
                bwtfxn(c->v);
            }


            buffer1_2[!bs][i] = c;
            Log::stage::chunk_written("stage_1", "buffer1_2", bs, i, c);
        }

        stats_stages_time[1] = clk->report();
        Log::stage::ended("read_file", stats_stages_time[1]);
        delete clk;
    }


    /**
     * Stage 2: MTF + zleWheeler functions applied to chunk
     * @param bs    buffer side
     */
    void stage_2(int bs);

    /**
     * Stage 3: Arithmetic encoder function applied to chunk
     * @param bs    buffer side
     */
    void stage_3(int bs);

    /**
     * Write output file with compressed chunks
     * @param bs        buffer side
     * @param outfile
     */
    void write_file(int bs, bwtzip::OutputFile &outfile);


    /**
     * Update pipeline statics after iteration
     * @param iter
     */
    void stats_update(int iter);

    /**
     * parallel bwtzip main function
     * @tparam T
     * @param argc
     * @param argv
     * @param executableName
     * @param algorithmName
     * @param bwtfxn
     */
    template<typename T>
    void pbwtzipMain(int argc, char *argv[], std::string executableName, std::string algorithmName, T bwtfxn) {
        using namespace std;
        /**
         * bwtCompress original function
         *
        template<typename T>
        std::vector<unsigned char> &bwtCompress(std::vector<unsigned char> &v, T bwtfxn) {
            using namespace std;
            using namespace bwtzip;

            clockStart();
            bwtfxn(v);      cout << "BWT:         " << clockReport() << endl;
            mtf2(v);        cout << "MTF-2:       " << clockReport() << endl;
            zleWheeler(v);  cout << "Wheeler ZLE: " << clockReport() << endl;
            arith(v);       cout << "Arith:       " << clockReport() << endl;

            return v;
        }
        */
        cout << "HERE3" <<endl;

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
        cout << argv[argc - 2] <<endl;
        cout << argv[argc - 1] <<endl;

        InputFile infile(argv[argc - 2]);
        OutputFile outfile(argv[argc - 1]);
        outfile.append(encodeULL(BWTZIP_SIG));
        if (LOG_PBWTZIP) {
            cout << "[PBWTZIP] BWT Algorithm: " << algorithmName << endl;
            cout << "[PBWTZIP] Chunk Size:    " << max_chunk_size << endl;
        }


        auto clk = new wClock();

        // set all cunk pointer to null in order to ensure initialization
        for (auto j = 0; j < 2; j++) {
            for (auto i = 0; i < BUFFER_SIZE; i++) {
                bufferR_1[j][i] = nullptr;
                buffer1_2[j][i] = nullptr;
                buffer2_3[j][i] = nullptr;
                buffer3_W[j][i] = nullptr;
            }
        }

        // buffer side
        int bs = 0;
        // reader   (r: bs)  w: !bs
        // stage 1   r: bs   w: !bs
        // stage 2   r: bs   w: !bs
        // writer    r: bs  (w: !bs)
        omp_set_nested(1);

        int iter = 0;
        while (ongoing_file_processing) {
            Log::pipeline::started(iter, bs);

#pragma omp parallel sections firstprivate(bs)
            {
#pragma omp section
                read_file(bs, infile, max_chunk_size);

#pragma omp section
                stage_1(bs, bwtfxn);

#pragma omp section
                stage_2(bs);

#pragma omp section
                stage_3(bs);

#pragma omp section
                write_file(bs, outfile);
            }
            if (LOG_STATISTICS) stats_update(iter);

            Log::pipeline::ended(iter, bs);

            // Toggle buffer side.
            // Side used for reading become the one for writing and vice versa on next iteration.
            bs = !bs;
            iter++;
        }
        if (LOG_PBWTZIP) printf("[PBWTZIP] Execution time: %f\n\n", clk->report());

        Log::pipeline::stats_print_summary(--iter, stats_lasted_longer_count, stats_time_averages);
        delete clk;
    }

}

#endif // End of: #ifndef PBWTZIP_HH
