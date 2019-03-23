#include "wclock.hh"
#include <string>
#include <wclock.hh>


using namespace std;

wClock::wClock(){
    start();
}

void wClock::start() {
    lastret = omp_get_wtime();
}

double wClock::report() {
    double t = lastret;
    lastret = omp_get_wtime();

    return lastret - t;
}

wClock::~wClock() {}
