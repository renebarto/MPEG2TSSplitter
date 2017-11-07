#include "media/TransportStream.hpp"

#include <iomanip>
#include "tools/Util.hpp"
#include "tools/Console.hpp"

using namespace std;
using namespace Media;

TransportStream::TransportStream(istream & stream, IStreamCallback * callback)
    : _stream(stream)
    , _packetIndex()
    , _verbose()
    , _TSBuffer(TSPacket::PacketSize)
    , _PATBuffer()
    , _PAT()
    , _PMTBuffer()
    , _PMT(callback)
    , _audioPID(PIDType::NULL_PACKET)
    , _videoPID(PIDType::NULL_PACKET)
    , _audioBuffer()
    , _audioPacket()
    , _videoBuffer()
    , _videoPacket()
{

}

bool TransportStream::ReadPacket(TSPacket & packet)
{
    size_t bytesLeft = TSPacket::PacketSize;
    size_t offset = 0;
    while (!_stream.bad() && !_stream.eof() && (bytesLeft > 0))
    {
        ssize_t bytesRead = _stream.readsome(reinterpret_cast<char *>(_TSBuffer.data() + offset), bytesLeft);
        if (bytesRead <= 0)
            break;
        bytesLeft -= bytesRead;
        offset += bytesRead;
    }
    if (_stream.bad() || _stream.eof() || (bytesLeft > 0))
        return false;

    packet.Parse(_TSBuffer);
    if (_verbose)
        packet.DisplayContents(_packetIndex);

    if (packet.HasError())
        return false;
    if (IsPSI(packet.PID()))
    {
        uint8_t startOffset = packet.PayloadStartOffset();
        if (packet.HasPayloadUnitStartIndicator())
        {
            uint8_t pointerField = packet.Data()[packet.PayloadStartOffset()];
            startOffset = static_cast<uint8_t>(packet.PayloadStartOffset() + 1 + pointerField);
        }
        if (packet.PID() == PIDType::PAT)
        {
            auto dataStart = packet.Data().begin() + startOffset;
            auto dataEnd = packet.Data().end();
            while ((dataEnd >= dataStart) && (*(dataEnd - 1) == 0xFF))
                --dataEnd;
            _PATBuffer.insert(_PATBuffer.end(), dataStart, dataEnd);
            if (_PAT.Parse(_PATBuffer))
            {
                if (_verbose)
                    _PAT.DisplayContents();
                _PATBuffer.clear();
            }
            else if (!_PAT.NeedMoreData())
                return false;
        } else if (packet.PID() == PIDType::CAT)
        {
            throw std::logic_error("CAT not implemented");
        } else if (packet.PID() == PIDType::TSDT)
        {
            throw std::logic_error("TSDT not implemented");
        } else if (packet.PID() == PIDType::ICIT)
        {
            throw std::logic_error("ICIT not implemented");
        } else if (_PAT.IsNIT(packet.PID()))
        {
            throw std::logic_error("NIT not implemented");
        } else if (_PAT.IsPMT(packet.PID()))
        {
            auto dataStart = packet.Data().begin() + startOffset;
            auto dataEnd = packet.Data().end();
            while ((dataEnd >= dataStart) && (*(dataEnd - 1) == 0xFF))
                --dataEnd;
            _PMTBuffer.insert(_PMTBuffer.end(), dataStart, dataEnd);
            if (_PMT.Parse(_PMTBuffer))
            {
                if (_verbose)
                    _PMT.DisplayContents();
                _PMTBuffer.clear();
            }
            else if (!_PMT.NeedMoreData())
                return false;
        }
    }
    else if ((_audioPID != PIDType::NULL_PACKET) && (_audioPID == packet.PID()))
    {
        auto dataStart = packet.Data().begin() + packet.PayloadStartOffset();
        auto dataEnd = packet.Data().end();
        _audioBuffer.insert(_audioBuffer.end(), dataStart, dataEnd);
        if (!_audioPacket.Parse(_audioBuffer, packet.HasPayloadUnitStartIndicator()))
            return false;
        _audioBuffer.clear();
    }
    else if ((_videoPID != PIDType::NULL_PACKET) && (_videoPID == packet.PID()))
    {
        auto dataStart = packet.Data().begin() + packet.PayloadStartOffset();
        auto dataEnd = packet.Data().end();
        _videoBuffer.insert(_videoBuffer.end(), dataStart, dataEnd);
        if (!_videoPacket.Parse(_videoBuffer, packet.HasPayloadUnitStartIndicator()))
            return false;
        _videoBuffer.clear();
    }
    ++_packetIndex;
    return true;
}

bool TransportStream::IsPSI(PIDType pid)
{
    switch (pid)
    {
        case PIDType::PAT:
            return true;
        case PIDType::CAT:
            return true;
        case PIDType::TSDT:
            return true;
    }
    return _PAT.IsNIT(pid) || _PAT.IsPMT(pid);
}

