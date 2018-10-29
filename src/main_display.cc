// main_display.cc - Main function for display
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bcell.hh"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cout << "USAGE: display filename" << endl;
        exit(EXIT_SUCCESS);
    }

    vector<unsigned char> v = readFile(argv[1]);
    Bcell cell = v;
    cell.print();
}
