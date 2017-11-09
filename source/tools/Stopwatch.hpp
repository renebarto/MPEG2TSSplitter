#pragma once

#include <time.h>
#include <string>
#include <iostream>

namespace Tools
{

class Stopwatch
{
public:
    Stopwatch();
    virtual ~Stopwatch();

    void Start();
    void Lap();
    timespec GetStartTime();
    timespec GetLapTime();
    timespec GetResolution();
    double GetElapsedTime();

    void PrintTo(std::ostream & stream) const;

protected:
    timespec startTime;
    timespec lapTime;
    timespec freq;
};

inline std::ostream & operator << (std::ostream & stream, const Stopwatch & stopwatch)
{
    stopwatch.PrintTo(stream);
    return stream;
}

} // namespace Tools
