#pragma once

#include <string>

namespace Media
{

class MPEG2Demultiplexor
{
public:
    MPEG2Demultiplexor() = delete;
    MPEG2Demultiplexor(const MPEG2Demultiplexor &) = delete;
    MPEG2Demultiplexor & operator= (const MPEG2Demultiplexor &) = delete;

    MPEG2Demultiplexor(const std::string & inputPath);

    int Run();

private:
    std::string _inputPath;
};

} // namespace Media
