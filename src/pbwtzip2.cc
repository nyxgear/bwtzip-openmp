#include "pbwtzip2.hh"

namespace pbwtzip2 {
    // num of assigned threads to stages, set them to default
    unsigned int num_thread_stage_1;
    unsigned int num_thread_stage_2;

    // will be set to the max between num_thread_stage_1, num_thread_stage_2, num_thread_stage_3
    unsigned int buffer_size;

    // initialize buffers with two sides
    cnk_t ***bufferR_1 = new cnk_t **[2];
    cnk_t ***buffer1_2 = new cnk_t **[2];
    cnk_t ***buffer2_W = new cnk_t **[2];


    bool read_completed = false;
    bool ongoing_file_processing = true;

    double stats_stages_time[5];
    int stats_lasted_longer_count[5];
    double stats_time_averages[5];
}

/**
 * Read file
 * @param bs                buffer side
 * @param infile            input file
 * @param max_chunk_size    maximum chunk size
 */
void pbwtzip2::read_file(int bs, bwtzip::InputFile &infile, const unsigned long max_chunk_size) {
    Log::pbwtzip::stage::started("read_file", bs);
    auto clk = new wClock();

    for (unsigned int i = 0; i < buffer_size; i++) {
        cnk_t *c;
        auto v = infile.extractAtMost(max_chunk_size);

        if (v.empty()) {
            // submit into the pipeline a "null chunk"
            if (LOG_STAGE) printf("[PBWTZIP][read_file] Read COMPLETED.");
            c = nullptr;
            pbwtzip2::read_completed = true;
        } else {
            // create chunk
            c = new cnk_t();

            // v
            c->v = v;

            // id
            c->id = 1;
            if (c->v.size() < bwtzip::MIN_LENGTH_TO_COMPRESS) {
                c->id = 0;
            }
        }

        bufferR_1[!bs][i] = c;
        Log::pbwtzip::stage::chunk_written("read_file", "bufferR_1", !bs, i, c);
    }
    stats_stages_time[0] = clk->report();
    Log::pbwtzip::stage::ended("read_file", stats_stages_time[0]);
    delete clk;
}

/**
 * Stage 2: MTF + zleWheeler + Arithmetic encoder function applied to chunk
 * @param bs    buffer side
 */
void pbwtzip2::stage_2(int bs) {
    Log::pbwtzip::stage::started("stage_2", bs);
    auto clk = new wClock();

#pragma omp parallel for num_threads(num_thread_stage_2) firstprivate(bs)
    for (unsigned int i = 0; i < buffer_size; i++) {
        cnk_t *c;

        c = buffer1_2[bs][i];
        Log::pbwtzip::stage::chunk_read("stage_2", "buffer1_2", bs, i, c);

        if (c != nullptr && c->id == 1) {
            bwtzip::mtf2(c->v);
            bwtzip::zleWheeler(c->v);
            bwtzip::arith(c->v);
        }

        buffer2_W[!bs][i] = c;
        Log::pbwtzip::stage::chunk_written("stage_2", "buffer2_W", !bs, i, c);
    }

    stats_stages_time[2] = clk->report();
    Log::pbwtzip::stage::ended("stage_2", stats_stages_time[2]);
    delete clk;
}

/**
 * Write output file with compressed chunks
 * @param bs        buffer side
 * @param outfile
 */
void pbwtzip2::write_file(int bs, bwtzip::OutputFile &outfile) {
    Log::pbwtzip::stage::started("write_file", bs);
    auto clk = new wClock();

    for (unsigned int i = 0; i < buffer_size; i++) {
        cnk_t *c;

        c = buffer2_W[bs][i];
        Log::pbwtzip::stage::chunk_read("write_file", "buffer2_W", bs, i, c);

        if (c != nullptr) {
            vector<unsigned char> id;
            id.push_back((unsigned char) c->id);

            outfile.append(bwtzip::encodeUL(c->v.size()));
            outfile.append(id);
            outfile.append(c->v);
            // free c memory
            delete c;
        } else if (read_completed) {
            // received a "null chunk" so, if reading is completed, file processing is terminated
            pbwtzip2::ongoing_file_processing = false;
            if (LOG_STAGE) printf("[PBWTZIP][write_file] Write COMPLETED.");
        }
    }

    stats_stages_time[3] = clk->report();
    Log::pbwtzip::stage::ended("write_file", stats_stages_time[3]);
    delete clk;
}


/**
 * Update pipeline statics after iteration
 * @param iter
 */
void pbwtzip2::stats_update(int iter) {
    double max_time = 0;
    int max_stage = 0;

    for (int i = 0; i < 4; i++) {
        if (stats_stages_time[i] > max_time) {
            max_time = stats_stages_time[i];
            max_stage = i;
        }
    }
    stats_lasted_longer_count[max_stage]++;

    if (iter == 0) {
        // first iteration, copy the times
        for (int i = 0; i < 4; i++)
            stats_time_averages[i] = stats_stages_time[i];
    } else {
        // iter > 0 hence 2nd or next iteration.
        // incremental mean  U_n = [X_n + (n - 1)*U_n-1]/n
        // iter is actually n - 1 since zero-initialized
        for (int i = 0; i < 4; i++)
            stats_time_averages[i] = (stats_stages_time[i] + (iter * stats_time_averages[i])) / (iter + 1);
    }
}
