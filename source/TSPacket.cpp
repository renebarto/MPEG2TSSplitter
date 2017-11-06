#include "TSPacket.hpp"

#include <iomanip>
#include <tools/SerializationImpl.hpp>
#include "tools/Util.hpp"
#include "tools/Console.hpp"

using namespace std;

namespace Media
{

static constexpr uint8_t SyncByte = 0x47;
static constexpr uint8_t ContinuityCountMask = 0x0F;
static constexpr uint8_t AdaptationFieldOffset = 0x04;
static constexpr uint8_t AdaptationFlagsOffset = 0x05;

TSPacket::TSPacket()
    : _data()
    , _packetHeader()
    , _transportErrorIndicator()
    , _payloadUnitStartIndicator()
    , _transportPriority()
    , _pid()
    , _scramblingControl()
    , _adaptationFieldControl()
    , _continuityCount()
    , _prevContinuityCount()
    , _adaptationFieldSize()
    , _discontinuityIndicator()
{
}

bool TSPacket::Parse(const std::vector<uint8_t> & buffer)
{
    _prevContinuityCount = ContinuityCount();
    if (buffer.size() != PacketSize)
        throw std::runtime_error("Incorrect packet size");
    _data.SetData(buffer);
    _packetHeader = static_cast<uint32_t>(_data.ReadBits(32));
    _data.Reset();
    if (static_cast<uint8_t>(_data.ReadBits(8)) != SyncByte)
        return false;
    _transportErrorIndicator = _data.ReadBit();
    _payloadUnitStartIndicator = _data.ReadBit();
    _transportPriority = _data.ReadBit();
    _pid = static_cast<PIDType>(_data.ReadBits(13));
    _scramblingControl = static_cast<ScrambingControl>(_data.ReadBits(2));
    _adaptationFieldControl = static_cast<AdaptationFieldControlType>(_data.ReadBits(2));
    _continuityCount = static_cast<uint8_t>(_data.ReadBits(4));
    _adaptationFieldSize = HasAdaptationField() ? static_cast<uint8_t>(_data.ReadBits(8)) : static_cast<uint8_t>(0);
    if (!IsValid())
        return false;
    if (HasAdaptationField() && !ParseAdaptationField())
        return false;
}

bool TSPacket::IsValid() const
{
    return (AdaptationFieldControl() != AdaptationFieldControlType::Reserved) &&
           (((AdaptationFieldControl() == AdaptationFieldControlType::AdaptationOnly) &&
             (AdaptationFieldSize() == (PacketSize - AdaptationFlagsOffset))) ||
            ((AdaptationFieldControl() == AdaptationFieldControlType::AdaptationAndPayload) &&
             (AdaptationFieldSize() < (PacketSize - AdaptationFlagsOffset))));
}

bool TSPacket::HasError() const
{
    return _transportErrorIndicator;
}

bool TSPacket::HasPayloadUnitStartIndicator() const
{
    return _payloadUnitStartIndicator;
}

bool TSPacket::HasTransportPriority() const
{
    return _transportPriority;
}

PIDType TSPacket::PID() const
{
    return _pid;
}

ScrambingControl TSPacket::TransportScramblingControl() const
{
    return _scramblingControl;
}

AdaptationFieldControlType TSPacket::AdaptationFieldControl() const
{
    return _adaptationFieldControl;
}

uint8_t TSPacket::ContinuityCount() const
{
    return _continuityCount;
}

uint8_t TSPacket::AdaptationFieldSize() const
{
    return _adaptationFieldSize;
}

uint8_t TSPacket::PayloadSize() const
{
    return HasPayload()
           ? (HasAdaptationField()
              ? static_cast<uint8_t>(PacketSize - AdaptationFlagsOffset - _adaptationFieldSize)
              : static_cast<uint8_t>(PacketSize - AdaptationFieldOffset))
           : static_cast<uint8_t>(0);
}

bool TSPacket::HasAdaptationField() const
{
    return (static_cast<uint8_t>(AdaptationFieldControl()) & static_cast<uint8_t>(AdaptationFieldControlType::AdaptationOnly)) != 0;
}

bool TSPacket::HasPayload() const
{
    return (static_cast<uint8_t>(AdaptationFieldControl()) & static_cast<uint8_t>(AdaptationFieldControlType::PayloadOnly)) != 0;
}

bool TSPacket::HasDiscontinuity() const
{
    return HasAdaptationField() && _discontinuityIndicator;
}

bool TSPacket::HasCounterDiscontinuity() const
{
    return HasPayload() && (ContinuityCount() != static_cast<uint8_t>((_prevContinuityCount + 1) & ContinuityCountMask));
}

bool TSPacket::IsDuplicate() const
{
    return HasPayload() && (ContinuityCount() == _prevContinuityCount);
}

uint8_t TSPacket::PayloadStartOffset() const
{
    return static_cast<uint8_t>(PacketSize - PayloadSize());
}

bool TSPacket::ParseAdaptationField()
{
    _discontinuityIndicator = _data.ReadBit();
    _data.ReadBits(7);
    _data.SkipBytes(static_cast<size_t>(_adaptationFieldSize - 1));
    return true;
}

void TSPacket::DisplayContents(size_t packetIndex) const
{
    Tools::DefaultConsole() << endl << "TS Packet" << endl<< endl;
    Tools::DefaultConsole() << "Packet index:     " << packetIndex << endl;
    Tools::DumpBytes(Data());
    Tools::DefaultConsole() << "PID:              " << Tools::Serialize(static_cast<uint16_t>(PID()), 16) << " (" << PID() << ")" << endl;
    Tools::DefaultConsole() << "Packet header:    " << Tools::Serialize(PacketHeader(), 16) << endl;
    Tools::DefaultConsole() << "Adaptation field: " << (HasAdaptationField() ? "Yes" : "No") << " Size: " << Tools::Serialize(AdaptationFieldSize(), 10) << endl;
    Tools::DefaultConsole() << "Payload field:    " << (HasPayload() ? "Yes" : "No") << " Size: " << Tools::Serialize(PayloadSize(), 10) << endl;
}
} // namespace Media
