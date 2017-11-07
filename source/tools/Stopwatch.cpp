#include "tools/Stopwatch.hpp"

#include <cstring>
#include <iomanip>
#include <sstream>
#include "tools/Util.hpp"

using namespace std;
using namespace Tools;

Stopwatch::Stopwatch()
{
    memset(&startTime, 0, sizeof(startTime));
    memset(&lapTime, 0, sizeof(lapTime));

    if (::clock_getres(CLOCK_REALTIME, &freq) != 0)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::Start()
{
    if (clock_gettime(CLOCK_REALTIME, &startTime) != 0)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void Stopwatch::Lap()
{
    if (clock_gettime(CLOCK_REALTIME, &lapTime) != 0)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

timespec Stopwatch::GetStartTime()
{
    return startTime;
}

timespec Stopwatch::GetLapTime()
{
    return lapTime;
}

timespec Stopwatch::GetResolution()
{
    return freq;
}

double Stopwatch::GetElapsedTime()
{
    double diffTime = 0;

    diffTime = (double)(lapTime.tv_sec - startTime.tv_sec) +
               1e-9 * (lapTime.tv_nsec - startTime.tv_nsec);
    return diffTime;
}

void Stopwatch::PrintTo(std::ostream & stream) const
{
    stream << " start = " << startTime.tv_sec << "." << setfill('0') << setw(9) << startTime.tv_nsec
           << " lap = " << lapTime.tv_sec << "." << setfill('0') << setw(9) << lapTime.tv_nsec;
}
