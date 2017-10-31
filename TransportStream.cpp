#include "TransportStream.hpp"

using namespace std;
using namespace Media;

TransportStream::TransportStream(istream & stream)
    : _stream(stream)
    , _packetIndex()
{

}

bool TransportStream::ReadPacket(TSPacket & packet)
{
    if (_stream.bad() || _stream.eof())
        return false;
    _stream.read(reinterpret_cast<char *>(packet.Data()), TSPacket::PacketSize);
    if (!_stream)
        return false;
    ++_packetIndex;
    return true;
}
