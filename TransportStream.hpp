#pragma once

#include <iostream>
#include "TSPacket.hpp"

namespace Media
{

class TransportStream
{
public:
    TransportStream() = delete;
    TransportStream(const TransportStream &) = delete;
    TransportStream & operator= (const TransportStream &) = delete;

    explicit TransportStream(std::istream & stream);

    bool ReadPacket(TSPacket & packet);
    size_t PacketIndex() const { return _packetIndex; }

private:
    std::istream & _stream;
    size_t _packetIndex;
};

} // namespace Media
