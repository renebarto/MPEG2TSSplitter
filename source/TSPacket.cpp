#include "TSPacket.hpp"

namespace Media
{

static constexpr uint8_t SyncByte = 0x47;                       // _data[0]
static constexpr uint8_t TransportErrorIndicatorMask = 0x80;    // _data[1]
static constexpr uint8_t PayloadUnitStartIndicatorMask = 0x40;  // _data[1]
static constexpr uint8_t TransportPriorityMask = 0x20;          // _data[1]
static constexpr uint8_t PIDMask = 0x1F;                        // _data[1] and complete _data[2]
static constexpr uint8_t TransportScramblingControlMask = 0xC0; // _data[3]
static constexpr uint8_t AdaptationFieldControlMask = 0x30;     // _data[3]
static constexpr uint8_t ContinuityCountMask = 0x0F;            // _data[3]
                                                                // _data[4] is adaptation field length if present
static constexpr uint8_t DiscontinuityIndicatorMask = 0x80;     // _data[5]

static constexpr uint8_t SyncByteOffset = 0x00;
static constexpr uint8_t FlagsAndPIDHighOffset = 0x01;
static constexpr uint8_t PIDLowOffset = 0x02;
static constexpr uint8_t ControlFlagsOffset = 0x03;
static constexpr uint8_t AdaptationFieldOffset = 0x04;
static constexpr uint8_t AdaptationFlagsOffset = 0x05;

enum AdaptationFieldType
{
    Reserved = 0x0,
    PayloadOnly = 0x1,
    AdaptationOnly = 0x2,
    AdaptationAndPayload = 0x3,
};

TSPacket::TSPacket()
    : _data()
    , _prevContinuityCount()
{
}

void TSPacket::Parse(const std::vector<uint8_t> & buffer)
{
    _prevContinuityCount = ContinuityCount();
    std::copy(buffer.begin(), buffer.end(), _data);
}

bool TSPacket::IsValid() const
{
    return (_data[SyncByteOffset] == SyncByte) &&
           (AdaptationFieldControl() != AdaptationFieldType::Reserved) &&
           (((AdaptationFieldControl() == AdaptationFieldType::AdaptationOnly) &&
             (AdaptationFieldSize() == (PacketSize - AdaptationFlagsOffset))) ||
            ((AdaptationFieldControl() == AdaptationFieldType::AdaptationAndPayload) &&
             (AdaptationFieldSize() < (PacketSize - AdaptationFlagsOffset))));
}

bool TSPacket::HasError() const
{
    return (_data[FlagsAndPIDHighOffset] & TransportErrorIndicatorMask) != 0;
}

bool TSPacket::HasPayloadUnitStartIndicator() const
{
    return (_data[FlagsAndPIDHighOffset] & PayloadUnitStartIndicatorMask) != 0;
}

bool TSPacket::HasTransportPriority() const
{
    return (_data[FlagsAndPIDHighOffset] & TransportPriorityMask) != 0;
}

uint16_t TSPacket::PID() const
{
    return _data[PIDLowOffset] | (_data[FlagsAndPIDHighOffset] & PIDMask) << 8;
}

uint8_t TSPacket::TransportScramblingControl() const
{
    return (_data[ControlFlagsOffset] & TransportScramblingControlMask) >> 6;
}

uint8_t TSPacket::AdaptationFieldControl() const
{
    return (_data[ControlFlagsOffset] & AdaptationFieldControlMask) >> 4;
}

uint8_t TSPacket::ContinuityCount() const
{
    return (_data[ControlFlagsOffset] & ContinuityCountMask) >> 0;
}

uint8_t TSPacket::AdaptationFieldSize() const
{
    return HasAdaptationField() ? _data[AdaptationFieldOffset] : static_cast<uint8_t>(0);
}

uint8_t TSPacket::PayloadSize() const
{
    return HasPayload()
           ? (HasAdaptationField()
              ? static_cast<uint8_t>(PacketSize - AdaptationFlagsOffset - _data[AdaptationFieldOffset])
              : static_cast<uint8_t>(PacketSize - AdaptationFieldOffset))
           : static_cast<uint8_t>(0);
}

bool TSPacket::HasAdaptationField() const
{
    return (AdaptationFieldControl() & AdaptationOnly) != 0;
}

bool TSPacket::HasPayload() const
{
    return (AdaptationFieldControl() & PayloadOnly) != 0;
}

bool TSPacket::HasDiscontinuity() const
{
    return HasAdaptationField() && (_data[AdaptationFlagsOffset] & DiscontinuityIndicatorMask) != 0;
}

bool TSPacket::HasCounterDiscontinuity() const
{
    return ContinuityCount() != static_cast<uint8_t>((_prevContinuityCount + 1) & ContinuityCountMask);
}

bool TSPacket::IsDuplicate() const
{
    return HasPayload() && (ContinuityCount() == _prevContinuityCount);
}

uint8_t TSPacket::PayloadStartOffset() const
{
    return PacketSize - PayloadSize();
}


} // namespace Media
