#pragma once

#include <iostream>
#include <deque>
#include <vector>
#include "TSPacket.hpp"
#include "PAT.hpp"

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

    bool IsPSI(uint16_t pid);
    const PAT & GetPAT() { return _PAT; }

    void DisplayPacketContents(const TSPacket & packet);

private:
    std::istream & _stream;
    size_t _packetIndex;

    std::vector<uint8_t> _TSBuffer;
    std::deque<uint8_t> _PATBuffer;
    PAT _PAT; // Program Association Table
    int _PMT; // Program Map Table
    int _CAT; // Conditional Access Table
    int _NIT; // Network Information Table
    int _TSDT; // Transport Stream Description Table
};

} // namespace Media
