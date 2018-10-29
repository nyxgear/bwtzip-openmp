// main_pngacc.cc - Main function for pngacc
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bcell.hh"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "USAGE: pngacc infile outfile" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    Bcell cell = v;
    cell.pngacc();
    v = cell;
    writeFile(v, argv[2]);
}
