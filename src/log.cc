
#include <log.hh>

#include "log.hh"

using namespace std;

namespace Log {
    std::ofstream csvfile;
}

void Log::open_csv_log(std::string filename) {
    csvfile.open(filename);
//    csvfile
//            << "Filename, "
//            << "Times_lasted_longer_read, Times_lasted_longer_stage1, Times_lasted_longer_stage2, Times_lasted_longer_stage3, Times_lasted_longer_write, "
//            << "Avg_exec_time_read, Avg_exec_time_stage1, Avg_exec_time_stage2, Avg_exec_time_stage3, Avg_exec_time_write,   "
//            << "Total_compression_time\n";
    csvfile << filename << ", ";
}

void Log::close_csv_log() {
    csvfile.close();
}


void Log::pipeline::started(int iter, int bs) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][pipeline] ---- Started iteration %d, BS %d\n", iter, bs);
}

void Log::pipeline::ended(int iter, int bs) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][pipeline] ---- Ended iteration %d, BS %d\n\n", iter, bs);
}

void Log::pipeline::stats_print_summary(int iter, int lasted_longer_count[], double time_averages[]) {
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
        for (int i = 0; i < 5; i++)
            csvfile << lasted_longer_count[i] << ", ";
        for (int i = 0; i < 5; i++)
            csvfile << time_averages[i] << ", ";
    }
}

void Log::pipeline::print_total_exec_time(double time) {
    if (LOG_PBWTZIP) printf("[PBWTZIP] Execution time: %f\n\n", time);
    if (LOG_STATISTICS_CSV) csvfile << time << "\n";
}

void Log::stage::started(string stage_name, int bs) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][%s] Started. BS: %d\n", stage_name.data(), bs);
}


void Log::stage::ended(string stage_name, double time) {
    if (LOG_PBWTZIP && LOG_STAGE) printf("[PBWTZIP][%s] Ended. Time of stage execution: %f\n", stage_name.data(), time);
}

void Log::stage::chunk_read(string stage_name, string buffer_name, int bs, int i, void *c) {
    if (LOG_PBWTZIP && LOG_STAGE && LOG_CHUNKS) {
        std::string nll = "";
        if (c == nullptr)
            nll = "NULL ";

        printf("[PBWTZIP][%s] %s[%d][%d]  Read %schunk.\n", stage_name.data(), buffer_name.data(), bs, i, nll.data());
    }
}


void Log::stage::chunk_written(string stage_name, string buffer_name, int bs, int i, void *c) {
    if (LOG_PBWTZIP && LOG_STAGE && LOG_CHUNKS) {
        std::string nll = "";
        if (c == nullptr)
            nll = "NULL ";

        printf("[PBWTZIP][%s] %s[%d][%d]  Written %schunk.\n", stage_name.data(), buffer_name.data(), bs, i,
               nll.data());
    }
}
