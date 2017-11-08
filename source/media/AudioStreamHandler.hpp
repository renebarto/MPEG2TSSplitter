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
    AudioStreamHandler(IDecoder::Ptr decoder, IStreamCallback * streamInfoCallback, bool verbose);

    bool Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator) override;

private:
    PESPacket _pesPacket;
    bool _verbose;
};

} // namespace Media
