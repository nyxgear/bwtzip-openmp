#include <iostream>
#include "catch.hpp"

#include "bwtzip.hh"
#include "bwtzip_suffixtree.hh"

using namespace bwtzip;

TEST_CASE("Human Genome Compression") {
    int argc = 3;
    char *argv[] = {
            "bwtzip",
            "../dataset/large_english_text/1400-0.txt",
            "../dataset/large_english_text_compressed/1400-0.txt",
            NULL
    };

    bwtzipMain(argc, argv, "bwtzip", "Ukkonen Suffix Tree", bwtStree);
}