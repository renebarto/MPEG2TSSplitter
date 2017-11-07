#pragma once

#include <cstdint>
#include <deque>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include "tools/BitBuffer.hpp"

namespace Media
{

enum class ProgramDescriptorTag : uint8_t
{
    Reserved1 = 0,
    Reserved2 = 1,
    VideoStream = 2,
    AudioStream = 3,
    Hierarchy = 4,
    Registration = 5,
    DataStreamAlignment = 6,
    TargetBackgroundGrid = 7,
    VideoWindow = 8,
    CA = 9,
    ISO639Language = 10,
    SystemClock = 11,
    MultiplexBufferUtilization = 12,
    Copyright = 13,
    MaximumBitrate = 14,
    PrivateDataIndicator = 15,
    SmoothingBuffer = 16,
    STD = 17,
    IBP = 18,
    ISO13818_6_1 = 19,
    ISO13818_6_2 = 20,
    ISO13818_6_3 = 21,
    ISO13818_6_4 = 22,
    ISO13818_6_5 = 23,
    ISO13818_6_6 = 24,
    ISO13818_6_7 = 25,
    ISO13818_6_8 = 26,
    MPEG4Video = 27,
    MPEG4Audio = 28,
    IOD = 29,
    SL = 30,
    FMC = 31,
    ExternalES_ID = 32,
    MuxCode = 33,
    FmxBUfferSizer = 34,
    MultiplexBuffer = 35,
    ReservedFirst = 36,
    ReservedLast = 63,
    UserPrivateFirst = 64,
    UserPrivateLast = 255,
};

inline std::ostream & operator << (std::ostream & stream, ProgramDescriptorTag tag)
{
    switch (tag)
    {
        case ProgramDescriptorTag::Reserved1:
            stream << "Reserved1"; return stream;
        case ProgramDescriptorTag::Reserved2:
            stream << "Reserved2"; return stream;
        case ProgramDescriptorTag::ISO639Language:
            stream << "Language"; return stream;
    }
    stream << "UNKNOWN (" << std::hex << std::setw(2) << std::setfill('0') << (int)tag << std::dec << ")";
    return stream;
}

class ProgramDescriptor
{
public:
    using Ptr = std::shared_ptr<ProgramDescriptor>;

    ProgramDescriptor();

    virtual bool Parse(Tools::BitBuffer & buffer);
    virtual bool IsValid() const;
    virtual void DisplayContents() const;

protected:
    ProgramDescriptorTag _tag;
    uint8_t _length;
    std::vector<uint8_t> _data;
};

using ProgramDescriptorList = std::vector<ProgramDescriptor::Ptr>;

} // namespace Media
