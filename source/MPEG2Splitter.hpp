#pragma once

#include <string>
#include "TSDefinitions.hpp"

namespace Media
{

class MPEG2Splitter
{
public:
    MPEG2Splitter() = delete;
    MPEG2Splitter(const MPEG2Splitter &) = delete;
    MPEG2Splitter & operator= (const MPEG2Splitter &) = delete;

    MPEG2Splitter(const std::string & inputPath);

    void SetAudioPID(PIDType pid);
    void SetVideoPID(PIDType pid);

    int Run();

private:
    std::string _inputPath;
    PIDType _audioPID;
    PIDType _videoPID;
};

} // namespace Media
