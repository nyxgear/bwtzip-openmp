#include "pbwtzip.hh"
#include "bwtzip_suffixtree.hh"

using namespace pbwtzip;

int main(int argc, char *argv[]) {
    pbwtzipMain(argc, argv, "pbwtzip", "Ukkonen Suffix Tree", bwtStree);
}
