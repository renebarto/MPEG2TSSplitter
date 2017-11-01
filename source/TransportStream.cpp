#include <iomanip>
#include <climits>
#include "TransportStream.hpp"

using namespace std;
using namespace Media;

TransportStream::TransportStream(istream & stream)
    : _stream(stream)
    , _packetIndex()
    , _TSBuffer(TSPacket::PacketSize)
    , _PATBuffer()
    , _PAT()
    , _PMT()
    , _CAT()
    , _NIT()
    , _TSDT()
{

}

bool TransportStream::ReadPacket(TSPacket & packet)
{
    if (_stream.bad() || _stream.eof())
        return false;
    size_t bytesRead = _stream.readsome(reinterpret_cast<char *>(_TSBuffer.data()), TSPacket::PacketSize);
    if (!_stream)
        return false;
    if (bytesRead < TSPacket::PacketSize)
        return false;
    packet.Parse(_TSBuffer);
    if (packet.HasError())
        return false;
    if (IsPSI(packet.PID()))
    {
        uint8_t pointerField = packet.Data()[packet.PayloadStartOffset()];
        uint8_t startOffset = static_cast<uint8_t>(packet.PayloadStartOffset() + 1 + pointerField);
        if (static_cast<PIDType>(packet.PID()) == PIDType::PAT)
        {
            _PATBuffer.insert(_PATBuffer.end(), packet.Data()[startOffset], packet.Data()[TSPacket::PacketSize]);
            if (_PAT.Parse(_PATBuffer))
            {

            }
        }
    }
    ++_packetIndex;
    return true;
}

bool TransportStream::IsPSI(uint16_t pid)
{
    switch (static_cast<PIDType>(pid))
    {
        case PIDType::PAT:
            return true;
        case PIDType::CAT:
            return true;
        case PIDType::TSDT:
            return true;
        default:
            if (pid == _PAT.GetPID_PMT())
                return true;
            else if (pid == _PAT.GetPID_NIT())
                return true;
            break;
    }
    return false;
}

template <typename T>
T SwapBytes(T value)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T value;
        uint8_t u8[sizeof(T)];
    } source, dest;

    source.value = value;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.value;
}

template <>
uint8_t SwapBytes(uint8_t value)
{
    return value;
}

template <>
uint16_t SwapBytes(uint16_t value)
{
    return static_cast<uint16_t>(((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8));
}

template <>
uint32_t SwapBytes(uint32_t value)
{
    return static_cast<uint32_t>(((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) |
                                 ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24));
}

string PIDToString(const PAT & pat, uint16_t pid)
{
    switch (static_cast<PIDType>(pid))
    {
        case PIDType::PAT:
            return "PAT";
        case PIDType::CAT:
            return "CAT";
        case PIDType::TSDT:
            return "TSDT";
        case PIDType::NULL_PACKET:
            return "NULL";
        default:
            if (pid == pat.GetPID_PMT())
                return "PMT";
            else if (pid == pat.GetPID_NIT())
                return "NIT";
            break;
    }
    return "UNKNOWN";
}

void DumpBytes(const uint8_t * data, size_t size)
{
    constexpr size_t BytesPerColumn = 16;
    size_t offset = 0;
    while (offset < size)
    {
        for (size_t i = 0; i < BytesPerColumn; ++i)
        {
            cout << hex << setw(2) << setfill('0') << uppercase << (int)data[offset + i] << " ";
        }
        for (size_t i = 0; i < BytesPerColumn; ++i)
        {
            cout << (isprint(data[offset + i]) ? (char)data[offset + i] : '.') << " ";
        }
        offset += BytesPerColumn;
        cout << endl;
    }
}

void TransportStream::DisplayPacketContents(const TSPacket & packet)
{
    cout << "Packet index:     " << PacketIndex() << endl;
    DumpBytes(packet.Data(), TSPacket::PacketSize);
    cout << "PID:              " << hex << setw(4) << setfill('0') << packet.PID() << dec << "(" << PIDToString(_PAT, packet.PID()) << ")" << endl;
    cout << "Packet header:    " << hex << setw(8) << setfill('0') << SwapBytes(packet.PacketHeader()) << dec << endl;
    cout << "Adaptation field: " << (packet.HasAdaptationField() ? "Yes" : "No") << " Size: " << (int)packet.AdaptationFieldSize() << endl;
    cout << "Payload field:    " << (packet.HasPayload() ? "Yes" : "No") << " Size: " << (int)packet.PayloadSize() << endl;
}
