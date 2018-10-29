// clock.cc - Time Functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "clock.hh"

void clockStart(bool doIt) {
    if (doIt) {
        clockReport();
    }
}

double clockReport() {
    static double lastret = 0;

    double ret = (double) clock() / CLOCKS_PER_SEC;

    double t = lastret;
    lastret = ret;
    
    return ret - t;
}

void clockPrint(bool doIt) {
    if (doIt) {
        cout << "Seconds elapsed: " << clockReport() << endl;
    }
}

unsigned long long rdtsc() {
#ifndef NO_RDTSC
    unsigned long long x;
    __asm__ volatile(".byte 0x0f,0x31" : "=A" (x));
    return x;
#else
    return 0;
#endif
}
