// main_bwtbs.cc - Main function for bwtbs
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
#include "bwtzip_suffixbentleysedgewick.hh"
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    bwtzipMain(argc, argv, "bwtbs", "Bentley-Sedgewick Tripartition Quicksort", bwtBentleySedgewick);
}
