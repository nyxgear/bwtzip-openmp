// bwtzip.cc - bwtzip functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"

using namespace std;
namespace bwtzip {
    double stats_functions_time[4];
    int stats_was_slowest_count[4];
    double stats_time_averages[4];
}

/**
 * Update pipeline statics after iteration
 * @param iter
 */
void bwtzip::stats_update(int iter) {
    double max_time = 0;
    int max_stage = 0;

    for (int i = 0; i < 4; i++) {
        if (stats_functions_time[i] > max_time) {
            max_time = stats_functions_time[i];
            max_stage = i;
        }
    }
    stats_was_slowest_count[max_stage]++;

    if (iter == 0) {
        // first iteration, copy the times
        for (int i = 0; i < 4; i++)
            stats_time_averages[i] = stats_functions_time[i];
    } else {
        // iter > 0 hence 2nd or next iteration.
        // incremental mean  U_n = [X_n + (n - 1)*U_n-1]/n
        // iter is actually n - 1 since zero-initialized
        for (int i = 0; i < 4; i++)
            stats_time_averages[i] = (stats_functions_time[i] + (iter * stats_time_averages[i])) / (iter + 1);
    }
}

vector<size_t> bwtzip::convertPosToLengths(const vector<size_t> &pos) {
    const size_t n = pos.size();

    vector<size_t> l(n);

    for (size_t i = 0; i < n; i++) {
        l[i] = n - pos[i];
    }

    return l;
}
