#include "pbwtzip2.hh"
#include "bwtzip_suffixtree.hh"

using namespace pbwtzip2;

int main(int argc, char *argv[]) {
    pbwtzip2Main(argc, argv, "pbwtzip", "Ukkonen Suffix Tree", bwtStree);
}
