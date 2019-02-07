#include <iostream>
#include "catch.hpp"

#include "bwtzip.hh"
#include "bwtzip_suffixtree.hh"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace bwtzip;
namespace fs = boost::filesystem;


string dir = "large_english_text";
string src_dir = "../dataset/" + dir;
string dst_dir = "compression_output/" + dir;

// human genome file names to look for in the dataset/large_english_text folder
//string file_names = "1400-0.txt 2city12.txt 6dfre10.txt cwgen11.txt flat10a.txt mtent13.txt pandp12.txt pg2656.txt pg5314.txt pg893.txt slstr10.txt trabi10.txt vbgle11a.txt wrnpc10.txt 1dfre10.txt 3dfre10.txt 766-0.txt dosie10.txt jargon-4.3.3.dos.txt nkrnn11.txt pg1023.txt pg3160.txt pg6130.txt pwprs10.txt suall10.txt twtp110.txt warw11.txt 2000010.txt 5dfre11.txt 7crmp10.txt dscmn10.txt lwmen10.txt otoos610.txt pg18.txt pg4238.txt pg891.txt sense11.txt tmwht10.txt twtp210.txt world02.txt";
string file_names = "1400-0.txt 2city12.txt 6dfre10.txt cwgen11.txt flat10a.txt mtent13.txt pandp12.txt pg2656.txt";


TEST_CASE("Large English Text Compression") {
    vector<string> files;

    // split file names into array
    boost::split(files, file_names, boost::is_any_of(" "));

    auto const dst_dir_p = fs::system_complete(dst_dir);

    // delete any previous output folder and its contents
    fs::remove_all(dst_dir_p);

    // create again the directory
    fs::create_directories(dst_dir_p);

    for(const std::string& file: files) {
        string src_file = src_dir + "/" + file;
        string dst_file = dst_dir + "/" + file;

        char *argv[] = {
                "bwtzip",
                const_cast<char*>(src_file.c_str()),
                const_cast<char*>(dst_file.c_str()),
                NULL
        };
        bwtzipMain(3, argv, "bwtzip", "Ukkonen Suffix Tree", bwtStree);
    }
}