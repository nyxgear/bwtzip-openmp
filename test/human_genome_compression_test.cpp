#include <iostream>
#include "catch.hpp"

#include "bwtzip.hh"
#include "bwtzip_suffixtree.hh"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace bwtzip;
namespace fs = boost::filesystem;


// human genome file names to look for in the dataset/human_genome folder
string file_names =
        "01hgp10a.txt 02hgp10a.txt 03hgp10a.txt 04hgp10a.txt 05hgp10a.txt 06hgp10a.txt 07hgp10a.txt 08hgp10a.txt 09hgp10a.txt 0xhgp10a.txt 0yhgp10a.txt 10hgp10a.txt 11hgp10a.txt 12hgp10a.txt 13hgp10a.txt 14hgp10a.txt 15hgp10a.txt 16hgp10a.txt 17hgp10a.txt 18hgp10a.txt 19hgp10a.txt 20hgp10a.txt 21hgp10a.txt 22hgp10a.txt";

string output_dir = "compression_output/human_genome";

TEST_CASE("Human Genome Compression") {
    vector<string> files;

    // split file names into array
    boost::split(files, file_names, boost::is_any_of(" "));

    auto const output_dir_p = fs::system_complete(output_dir);

    // delete any previous output folder and its contents
    fs::remove_all(output_dir_p);

    // create again the directory
    fs::create_directories(output_dir_p);


    char *argv[] = {
            "bwtzip",
            "../dataset/human_genome/0xhgp10a.txt",
            "compression_output/human_genome/1400-0.txt",
            NULL};

    bwtzipMain(3, argv, "bwtzip", "Ukkonen Suffix Tree", bwtStree);
}