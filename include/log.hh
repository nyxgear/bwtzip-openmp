#ifndef LOG_HH
#define LOG_HH

#define LOG_PBWTZIP 1
#define LOG_STAGE 0
#define LOG_CHUNKS 0
#define LOG_TEST 0
#define LOG_STATISTICS 1

#define LOG_STATISTICS_CSV 1

#include <cstdio>
#include <string>
#include <fstream>

namespace Log {
    using namespace std;
    extern std::ofstream csvfile;

    void open_csv_log(std::string filename);

    void close_csv_log();


    namespace pipeline {
        void started(int iter, int bs);

        void ended(int iter, int bs);

        void stats_print_summary(int iter, int lasted_longer_count[], double time_averages[]);

        void print_total_exec_time(double time);
    }
    namespace stage {
        void started(string stage_name, int bs);

        void ended(string stage_name, double time);

        void chunk_read(string stage_name, string buffer_name, int bs, int i, void *c);

        void chunk_written(string stage_name, string buffer_name, int bs, int i, void *c);
    }
}

#endif // End of: #ifndef LOG_HH
