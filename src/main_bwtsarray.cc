// main_bwtsarray.cc - Main function for bwtsarray
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip.hh"
#include "bwtzip_suffixarray.hh"
using namespace std;
using namespace bwtzip;

int main(int argc, char *argv[]) {
    bwtzipMain(argc, argv, "bwtsarray", "Manber-Myers Suffix Array", bwtSarray);
}
