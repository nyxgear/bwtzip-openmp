// These are portable functions which time how long it takes
// for a function to execute based on wall clock.
// clockStart() starts a timer.
// clockReport() will return how many seconds have elapsed, and
// will also reset the timer to 0.

#ifndef WCLOCK_HH
#define WCLOCK_HH

#include <omp.h>
#include <string>

using namespace std;

class wClock {
public:
    wClock();

    ~wClock();

    void start();

    double report();

private:
    double lastret = 0;
};


#endif // End of: #ifndef WCLOCK_HH