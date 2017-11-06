#pragma once

#include <iostream>
#include <deque>
#include <vector>
#include "TSPacket.hpp"
#include "PAT.hpp"
#include "PMT.hpp"
#include "PESPacket.hpp"

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

    void SetAudioPID(PIDType pid)
    {
        _audioPID = pid;
    }
    void SetVideoPID(PIDType pid)
    {
        _videoPID = pid;
    }

    bool IsPSI(PIDType pid);
    const PAT & GetPAT() { return _PAT; }

private:
    std::istream & _stream;
    size_t _packetIndex;
    bool _verbose;

    std::vector<uint8_t> _TSBuffer;
    std::deque<uint8_t> _PATBuffer;
    PAT _PAT; // Program Association Table
    std::deque<uint8_t> _PMTBuffer;
    PMT _PMT; // Program Map Table
    PIDType _audioPID;
    PIDType _videoPID;
    std::deque<uint8_t> _audioBuffer;
    PESPacket _audioPacket;
    std::deque<uint8_t> _videoBuffer;
    PESPacket _videoPacket;
};

} // namespace Media
