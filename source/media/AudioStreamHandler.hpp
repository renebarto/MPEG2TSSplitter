#pragma once

#include <cstdint>
#include "media/IPIDDataHandler.hpp"
#include "media/PESPacket.hpp"

namespace Media
{

class IStreamCallback;

class AudioStreamHandler : public IPIDDataHandler
{
public:
    AudioStreamHandler(IDecoder::Ptr decoder);

    bool Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator) override;

private:
    PESPacket _pesPacket;
};

} // namespace Media
