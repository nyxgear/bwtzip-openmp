#include "pbwtzip.hh"

/**
 * Read file
 * @param bs                buffer side
 * @param infile            input file
 * @param max_chunk_size    maximum chunk size
 */
void pbwtzip::read_file(int bs, bwtzip::InputFile &infile, const unsigned long max_chunk_size) {
    Log::stage::started("read_file", bs);
    auto clk = new wClock();

    for (int i = 0; i < BUFFER_SIZE; i++) {
        cnk_t *c;
        auto v = infile.extractAtMost(max_chunk_size);

        if (v.empty()) {
            // submit into the pipeline a "null chunk"
            if (LOG_STAGE) printf("[PBWTZIP][read_file] Read COMPLETED.");
            c = nullptr;
            pbwtzip::read_completed = true;
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
        Log::stage::chunk_written("read_file", "bufferR_1", !bs, i, c);
    }
    stats_stages_time[0] = clk->report();
    Log::stage::ended("read_file", stats_stages_time[0]);
    delete clk;
}

/**
     * Stage 2: MTF + zleWheeler functions applied to chunk
     * @param bs    buffer side
     */
void pbwtzip::stage_2(int bs) {
    Log::stage::started("stage_2", bs);
    auto clk = new wClock();

#pragma omp parallel for num_threads(NUM_THREAD_STAGE_2) firstprivate(bs)
    for (auto i = 0; i < BUFFER_SIZE; i++) {
        cnk_t *c;

        c = buffer1_2[bs][i];
        Log::stage::chunk_read("stage_2", "buffer1_2", bs, i, c);

        if (c != nullptr && c->id == 1) {
            bwtzip::mtf2(c->v);
            bwtzip::zleWheeler(c->v);
        }

        buffer2_3[!bs][i] = c;
        Log::stage::chunk_written("stage_2", "buffer2_3", !bs, i, c);
    }

    stats_stages_time[2] = clk->report();
    Log::stage::ended("stage_2", stats_stages_time[2]);
    delete clk;
}


/**
 * Stage 3: Arithmetic encoder function applied to chunk
 * @param bs    buffer side
 */
void pbwtzip::stage_3(int bs) {
    Log::stage::started("stage_3", bs);
    auto clk = new wClock();

#pragma omp parallel for num_threads(NUM_THREAD_STAGE_3) firstprivate(bs)
    for (auto i = 0; i < BUFFER_SIZE; i++) {
        cnk_t *c;

        c = buffer2_3[bs][i];
        Log::stage::chunk_read("stage_3", "buffer2_3", bs, i, c);

        if (c != nullptr && c->id == 1) {
            bwtzip::arith(c->v);
        }

        buffer3_W[!bs][i] = c;
        Log::stage::chunk_written("stage_3", "buffer3_W", !bs, i, c);
    }

    stats_stages_time[3] = clk->report();
    Log::stage::ended("stage_3", stats_stages_time[3]);
    delete clk;
}

/**
 * Write output file with compressed chunks
 * @param bs        buffer side
 * @param outfile
 */
void pbwtzip::write_file(int bs, bwtzip::OutputFile &outfile) {
    Log::stage::started("write_file", bs);
    auto clk = new wClock();

    for (auto i = 0; i < BUFFER_SIZE; i++) {
        cnk_t *c;

        c = buffer3_W[bs][i];
        Log::stage::chunk_read("write_file", "buffer3_W", bs, i, c);

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
            pbwtzip::ongoing_file_processing = false;
            if (LOG_STAGE) printf("[PBWTZIP][write_file] Write COMPLETED.");
        }
    }

    stats_stages_time[4] = clk->report();
    Log::stage::ended("write_file", stats_stages_time[4]);
    delete clk;
}


/**
 * Update pipeline statics after iteration
 * @param iter
 */
void pbwtzip::stats_update(int iter) {
    double max_time = 0;
    int max_stage = 0;

    for (int i = 0; i < 5; i++) {
        if (stats_stages_time[i] > max_time) {
            max_time = stats_stages_time[i];
            max_stage = i;
        }
    }
    stats_lasted_longer_count[max_stage]++;

    if (iter == 0) {
        // first iteration, copy the times
        for (int i = 0; i < 5; i++)
            stats_time_averages[i] = stats_stages_time[i];
    } else {
        // iter > 0 hence 2nd or next iteration.
        // incremental mean  U_n = [X_n + (n - 1)*U_n-1]/n
        // iter is actually n - 1 since zero-initialized
        for (int i = 0; i < 5; i++)
            stats_time_averages[i] = (stats_stages_time[i] + (iter * stats_time_averages[i])) / (iter + 1);
    }
}
