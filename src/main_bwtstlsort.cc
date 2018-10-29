// main_bwtstlsort.cc - Main function for bwtstlsort
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
#include "bwtzip_suffixstlsort.hh"
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    bwtzipMain(argc, argv, "bwtstlsort", "STL Sort", bwtSuffixStlSort);
}
