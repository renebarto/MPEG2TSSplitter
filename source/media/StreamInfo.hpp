#pragma once

#include "media/StreamType.hpp"
#include "media/TSDefinitions.hpp"
#include "media/ProgramDescriptor.hpp"

namespace Media
{

class StreamInfo
{
public:
    StreamInfo();

    bool Parse(Tools::BitBuffer & buffer);
    bool IsValid() const;

    bool IsAudioStream() const;
    bool IsVideoStream() const;
    StreamType Type() const { return _type; }
    PIDType PID() const { return _pid; }

    void DisplayContents() const;

private:
    StreamType _type;
    PIDType _pid;
    ProgramDescriptorList _descriptors;
};

using StreamInfoList = std::vector<StreamInfo>;

} // namespace Media
