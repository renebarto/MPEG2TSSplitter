#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "media/TSPacket.hpp"
#include "media/PESPacket.hpp"
#include "media/IStreamCallback.hpp"
#include "media/IDecoder.hpp"
#include "media/PMTHandler.hpp"
#include "media/AudioStreamHandler.hpp"
#include "media/VideoStreamHandler.hpp"

namespace Media
{

using StreamMap = std::map<PIDType, IPIDDataHandler::Ptr>;

class TransportStream
{
public:
    TransportStream() = delete;
    TransportStream(const TransportStream &) = delete;
    TransportStream & operator= (const TransportStream &) = delete;

    explicit TransportStream(std::istream & stream, IStreamCallback * callback, bool verbose);

    void Initialize();

    bool ReadPacket(TSPacket & packet);
    bool ParsePacket(TSPacket & packet);
    size_t PacketIndex() const { return _packetIndex; }

    void AddStreamHandler(PIDType pid, IPIDDataHandler::Ptr handler);

private:
    std::istream & _stream;
    IStreamCallback * _callback;
    bool _verbose;
    size_t _packetIndex;
    StreamMap  _streamMap;

    std::vector<uint8_t> _TSBuffer;
};

} // namespace Media
