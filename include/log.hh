#ifndef LOG_HH
#define LOG_HH

#define LOG_SUFFIX_TREE 0
#define LOG_DFS 0
#define LOG_BWTZIP 0
#define LOG_BWTUNZIP 0


#define LOG_PBWTZIP 0
#define LOG_STAGE 0
#define LOG_CHUNKS 0
#define LOG_TEST 0
#define LOG_STATISTICS 0

// when enable, a csv file line will be created and output on the print_exec_statics_csv_line() invocation.
// first line of CSV file:
// BWTZIP
// Filename, Chunk_size, Iterations, Times_lasted_longer_function1, Times_lasted_longer_function2, Times_lasted_longer_function3, Times_lasted_longer_function4, Avg_exec_time_function1, Avg_exec_time_function2, Avg_exec_time_function3, Avg_exec_time_function4, Total_compression_time
// PBWTZIP
// Filename, Threads_configuration, Chunk_size, Iterations, Times_lasted_longer_read, Times_lasted_longer_stage1, Times_lasted_longer_stage2, Times_lasted_longer_stage3, Times_lasted_longer_write, Avg_exec_time_read, Avg_exec_time_stage1, Avg_exec_time_stage2, Avg_exec_time_stage3, Avg_exec_time_write, Total_compression_time
#define LOG_STATISTICS_CSV 1

#include <cstdio>
#include <string>
#include <fstream>

namespace Log {
    using namespace std;

    namespace bwtzip {
        void stats_print_summary(int iter, int lasted_longer_count[], double time_averages[]);
        void print_total_exec_time(double time);
        namespace csv {
            extern std::stringstream csv_line;

            void start_new_line(string filename, unsigned long chunk_size);

            void print_statistics_line();
        }
    }

    namespace pbwtzip {
        void started(int iter, int bs);

        void ended(int iter, int bs);

        void stats_print_summary(int iter, int lasted_longer_count[], double time_averages[]);

        void print_total_exec_time(double time);

        namespace stage {
            void started(string stage_name, int bs);

            void ended(string stage_name, double time);

            void chunk_read(string stage_name, string buffer_name, int bs, int i, void *c);

            void chunk_written(string stage_name, string buffer_name, int bs, int i, void *c);
        }
        namespace csv {
            extern std::stringstream csv_line;

            void start_new_line(string filename, char *thread_config, unsigned long chunk_size);

            void print_statistics_line();
        }
    }
}

#endif // End of: #ifndef LOG_HH
