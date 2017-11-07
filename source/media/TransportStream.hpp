#pragma once

#include <iostream>
#include <deque>
#include <vector>
#include "media/TSPacket.hpp"
#include "media/PAT.hpp"
#include "media/PMT.hpp"
#include "media/PESPacket.hpp"
#include "media/IStreamCallback.hpp"
#include "media/IDecoder.hpp"

namespace Media
{

class TransportStream
{
public:
    TransportStream() = delete;
    TransportStream(const TransportStream &) = delete;
    TransportStream & operator= (const TransportStream &) = delete;

    explicit TransportStream(std::istream & stream, IStreamCallback * callback);

    void SetVerbose(bool value) { _verbose = value; }
    bool ReadPacket(TSPacket & packet);
    size_t PacketIndex() const { return _packetIndex; }

    void SetAudioPID(PIDType pid, IDecoder::Ptr decoder)
    {
        _audioPID = pid;
        _audioPacket.SetDecoder(decoder);
    }
    void SetVideoPID(PIDType pid, IDecoder::Ptr decoder)
    {
        _videoPID = pid;
        _videoPacket.SetDecoder(decoder);
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
