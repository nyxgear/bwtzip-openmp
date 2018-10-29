// main_bwtzip.cc - Main function for bwtzip
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
#include "bwtzip_suffixtree.hh"
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    bwtzipMain(argc, argv, "bwtzip", "Ukkonen Suffix Tree", bwtStree);
}
