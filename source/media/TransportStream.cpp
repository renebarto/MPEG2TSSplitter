#include "media/TransportStream.hpp"

#include "tools/Util.hpp"
#include "tools/Console.hpp"
#include "media/PATHandler.hpp"
#include "media/CATHandler.hpp"
#include "media/TSDTHandler.hpp"
#include "media/ICITHandler.hpp"
#include "media/Logging.hpp"

using namespace std;
using namespace Media;

TransportStream::TransportStream(istream & stream, IStreamCallback * callback)
    : _stream(stream)
    , _callback(callback)
    , _packetIndex()
    , _streamMap()
    , _TSBuffer(TSPacket::PacketSize)
{

}

void TransportStream::Initialize()
{
    _streamMap.insert(pair<PIDType, IPIDDataHandler::Ptr>(PIDType::PAT, make_shared<PATHandler>(_callback)));
    _streamMap.insert(pair<PIDType, IPIDDataHandler::Ptr>(PIDType::CAT, make_shared<CATHandler>()));
    _streamMap.insert(pair<PIDType, IPIDDataHandler::Ptr>(PIDType::TSDT, make_shared<TSDTHandler>()));
    _streamMap.insert(pair<PIDType, IPIDDataHandler::Ptr>(PIDType::ICIT, make_shared<ICITHandler>()));
}

bool TransportStream::HaveHandlerForPID(PIDType pid) const
{
    return (_streamMap.find(pid) != _streamMap.end());
}

void TransportStream::AddStreamHandler(PIDType pid, IPIDDataHandler::Ptr handler)
{
    if (!HaveHandlerForPID(pid))
        _streamMap.insert(pair<PIDType, IPIDDataHandler::Ptr>(pid, handler));
}

bool TransportStream::ReadPacket()
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
    return true;
}

bool TransportStream::ParsePacket(TSPacket & packet)
{
    if (!packet.Parse(_TSBuffer))
        return false;
    if (VerboseLogging())
    {
        Tools::DumpBytes(_TSBuffer.begin(), _TSBuffer.end());
        packet.DisplayContents(_packetIndex);
    }

    if (packet.HasError())
        return false;
    if (_streamMap.find(packet.PID()) != _streamMap.end())
    {
        auto dataStart = _TSBuffer.begin() + packet.PayloadStartOffset();
        auto dataEnd = _TSBuffer.end();
        if (!_streamMap[packet.PID()]->Parse(dataStart, dataEnd, packet.HasPayloadUnitStartIndicator()))
            return false;
    }
    ++_packetIndex;
    return true;
}

