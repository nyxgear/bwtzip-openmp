// clock.hh - Time Functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// These are portable functions which time how long it takes
// for a function to execute.
// clockStart() starts a timer. clockStart(false) does nothing.
// clockReport() will return how many seconds have elapsed, and
// will also reset the timer to 0.
// clockPrint() prints how many seconds have elapsed, and will
// also reset the timer to 0. clockPrint(false) does nothing.

#ifndef CLOCK_HH
#define CLOCK_HH

#include <string>
#include <ctime>
#include <iostream>
using namespace std;

void clockStart(bool doIt = true);
double clockReport();
void clockPrint(bool doIt = true);

unsigned long long rdtsc();

class Clock {
public:
    Clock(string name) : name(name), lastret(0) { report(); }
    void print() {
        cout << "Clock \"" << name << "\"; Seconds elapsed: " << report() << endl;
    }
    double report() {
        double t = lastret;
        lastret = (double) clock() / CLOCKS_PER_SEC;
        return lastret - t;
    }

private:
    string name;
    double lastret;
};

#endif // End of: #ifndef CLOCK_HH
