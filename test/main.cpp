#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "stdio.h"
#include "pbwtzip.hh"
#include "bwtzip_suffixtree.hh"
#include "bwtunzip.hh"

using namespace bwtzip;

TEST_CASE("Compressed file matches original file") {
    const char *original_file_path = "../test/data_sample.txt";
    const char *compressed_file_path = "./data_sample_compressed.bwtzip";
    const char *decompressed_file_path = "./data_sample_decompressed.txt";

    // delete files if they exist
    remove(compressed_file_path);
    remove(decompressed_file_path);


    /**
     * Compress file
     */
    char *argv_zip[5] = {NULL, NULL, NULL, NULL, NULL};
    int argc_zip = sizeof(argv_zip) / sizeof(char *) - 1;

    argv_zip[0] = (char *) "pbwtzip";
    argv_zip[1] = (char *) "409600";
    // argv[1] = (char *) "5242880"; // MAX_CHUNK_SIZE
    argv_zip[2] = (char *) original_file_path;
    argv_zip[3] = (char *) compressed_file_path;

    std::cout << "[TEST] Compressing " << original_file_path << " to " << compressed_file_path << std::endl;
    pbwtzip::pbwtzipMain(argc_zip, argv_zip, "bwtzip", "Ukkonen Suffix Tree", bwtStree);


    /**
     * Decompress file
     */
    char *argv_unzip[4] = {NULL, NULL, NULL, NULL};
    int argc_unzip = sizeof(argv_unzip) / sizeof(char *) - 1;

    argv_unzip[0] = (char *) "bwtunzip";
    argv_unzip[1] = (char *) compressed_file_path;
    argv_unzip[2] = (char *) decompressed_file_path;

    std::cout << "[TEST] Decompressing " << compressed_file_path << " to " << decompressed_file_path << std::endl;
    bwtunzipMain(argc_unzip, argv_unzip);


    /**
     * Test decompressed files match original ones
     */
    InputFile original_file(original_file_path);
    InputFile decompressed_file(decompressed_file_path);

    bool original_decompressed_match = true;
    while (original_decompressed_match) {
        std::vector<unsigned char> org_v = original_file.extractAtMost(MAX_CHUNK_SIZE);
        std::vector<unsigned char> dec_v = decompressed_file.extractAtMost(MAX_CHUNK_SIZE);

        if (org_v.empty())
            break;

        original_decompressed_match = org_v == dec_v;
    }
    std::cout << "[TEST] Testing match of original and decompressed file" << std::endl;
    REQUIRE(original_decompressed_match);
}
