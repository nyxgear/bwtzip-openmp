#include "log.hh"

#include <iostream>
#include <log.hh>
#include <sstream>


using namespace std;

namespace Log {
    namespace bwtzip {
        namespace csv {
            std::stringstream csv_line;
        }
    }
    namespace pbwtzip {
        namespace csv {
            std::stringstream csv_line;
        }
    }
}

void Log::pbwtzip::csv::start_new_line(string filename, char *thread_config, unsigned long chunk_size) {
    csv::csv_line << filename << ", " << thread_config << ", " << chunk_size << ", ";
}

void Log::pbwtzip::csv::print_statistics_line() {
    std::cout << csv::csv_line.str() << std::endl;
}

void Log::pbwtzip::started(int iter, int bs) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][pbwtzip] ---- Started iteration %d, BS %d\n", iter, bs);
}

void Log::pbwtzip::ended(int iter, int bs) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][pbwtzip] ---- Ended iteration %d, BS %d\n\n", iter, bs);
}

void Log::pbwtzip::stats_print_summary(int iter, int lasted_longer_count[], double time_averages[]) {
    if (LOG_STATISTICS) {
        string stage[5] = {"read_file  ", "stage_1    ", "stage_2    ", "stage_3    ", "write_file "};

        printf("*************************************************\n");
        printf(" PIPELINE STATISTICS                             \n");
        printf("                                                 \n");
        printf(" Total pipeline cycles: %d                       \n", iter + 1);
        printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
        printf(" Times the stage was the last terminating in a   \n");
        printf(" cycle with respect the others                   \n");
        for (int i = 0; i < 5; i++)
            printf(" %s %d     \n", stage[i].c_str(), lasted_longer_count[i]);

        printf("\n‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
        printf(" Stage average cycle execution time              \n");
        for (int i = 0; i < 5; i++)
            printf(" %s %f     \n", stage[i].c_str(), time_averages[i]);
        printf("*************************************************\n");
    }

    if (LOG_STATISTICS_CSV) {
        csv::csv_line << iter + 1 << ", ";

        for (int i = 0; i < 5; i++)
            csv::csv_line << lasted_longer_count[i] << ", ";
        for (int i = 0; i < 5; i++)
            csv::csv_line << time_averages[i] << ", ";
    }
}

void Log::pbwtzip::print_total_exec_time(double time) {
    if (LOG_PBWTZIP) printf("[PBWTZIP] Execution time: %f\n\n", time);
    if (LOG_STATISTICS_CSV) csv::csv_line << time;
}

void Log::pbwtzip::stage::started(string stage_name, int bs) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][%s] Started. BS: %d\n", stage_name.data(), bs);
}


void Log::pbwtzip::stage::ended(string stage_name, double time) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][%s] Ended. Time of stage execution: %f\n", stage_name.data(), time);
}

void Log::pbwtzip::stage::chunk_read(string stage_name, string buffer_name, int bs, int i, void *c) {
    if (LOG_PBWTZIP && LOG_STAGE && LOG_CHUNKS) {
        std::string nll = "";
        if (c == nullptr)
            nll = "NULL ";

        printf("[PBWTZIP][%s] %s[%d][%d]  Read %schunk.\n", stage_name.data(), buffer_name.data(), bs, i, nll.data());
    }
}


void Log::pbwtzip::stage::chunk_written(string stage_name, string buffer_name, int bs, int i, void *c) {
    if (LOG_PBWTZIP && LOG_STAGE && LOG_CHUNKS) {
        std::string nll = "";
        if (c == nullptr)
            nll = "NULL ";

        printf("[PBWTZIP][%s] %s[%d][%d]  Written %schunk.\n", stage_name.data(), buffer_name.data(), bs, i,
               nll.data());
    }
}

void Log::bwtzip::stats_print_summary(int iter, int *lasted_longer_count, double *time_averages) {
    if (LOG_STATISTICS) {
        string function[4] = {"BWT          ", "MTF-2        ", "Wheeler ZLE  ", "Arith        "};

        printf("*************************************************\n");
        printf(" FUNCTIONS STATISTICS                            \n");
        printf("                                                 \n");
        printf(" Total iterations: %d                            \n", iter + 1);
        printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
        printf(" Times the function was the slowest with respect \n");
        printf(" to the others during an iteration               \n");
        for (int i = 0; i < 4; i++)
            printf(" %s %d     \n", function[i].c_str(), lasted_longer_count[i]);

        printf("\n‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
        printf(" Function average execution time                 \n");
        for (int i = 0; i < 5; i++)
            printf(" %s %f     \n", function[i].c_str(), time_averages[i]);
        printf("*************************************************\n");
    }

    if (LOG_STATISTICS_CSV) {
        csv::csv_line << iter + 1 << ", ";

        for (int i = 0; i < 4; i++)
            csv::csv_line << lasted_longer_count[i] << ", ";
        for (int i = 0; i < 4; i++)
            csv::csv_line << time_averages[i] << ", ";
    }


}


void Log::bwtzip::csv::start_new_line(string filename, unsigned long chunk_size) {
    csv::csv_line << filename << ", " << chunk_size << ", ";
}

void Log::bwtzip::csv::print_statistics_line() {
    std::cout << csv::csv_line.str() << std::endl;
}

void Log::bwtzip::print_total_exec_time(double time) {
    if (LOG_BWTZIP) printf("[BWTZIP] Execution time: %f\n\n", time);
    if (LOG_STATISTICS_CSV) csv::csv_line << time;
}


