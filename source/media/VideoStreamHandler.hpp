#pragma once

#include <cstdint>
#include <deque>
#include "PESPacket.hpp"

namespace Media
{

using TSBufferIterator = std::vector<uint8_t>::const_iterator;

class VideoStreamHandler
{
public:
    VideoStreamHandler();

    void SetDecoder(IDecoder::Ptr decoder)
    {
        _pesPacket.SetDecoder(decoder);
    }
    bool Parse(const TSBufferIterator & start, const TSBufferIterator & end, bool hasStartIndicator);

private:
    std::deque<uint8_t> _buffer;
    PESPacket _pesPacket;
};

} // namespace Media
