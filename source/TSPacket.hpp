#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Media
{

enum class PIDType
{
    PAT = 0x0000,       // Program Association Table
    CAT = 0x0001,       // Conditional Access Table
    TSDT = 0x0002,      // Transport Stream Description Table
    NULL_PACKET = 0x1FFF,
};

enum ESType
{
    Unknown = 0,
};

class TSPacket
{
public:
    static constexpr size_t PacketSize = 188;

    TSPacket();
    TSPacket(const TSPacket &) = delete;
    TSPacket & operator= (const TSPacket &) = delete;

    void Parse(const std::vector<uint8_t> & buffer);

    const uint8_t * Data() const { return _data; }
    uint8_t * Data() { return _data; }
    ESType StreamType() { return ESType::Unknown; }

    bool IsValid() const;
    bool IsAudio() const { return false; }
    bool IsVideo() const { return false; }

    uint32_t PacketHeader() const { return *reinterpret_cast<const uint32_t *>(_data); }
    bool HasError() const;
    bool HasPayloadUnitStartIndicator() const;
    bool HasTransportPriority() const;
    uint16_t PID() const;
    uint8_t TransportScramblingControl() const;
    uint8_t AdaptationFieldControl() const;
    uint8_t ContinuityCount() const;
    uint8_t AdaptationFieldSize() const;
    uint8_t PayloadSize() const;
    bool HasAdaptationField() const;
    bool HasPayload() const;
    bool HasDiscontinuity() const;
    bool HasCounterDiscontinuity() const;
    bool IsDuplicate() const;
    uint8_t PayloadStartOffset() const;

private:
    uint8_t _data[PacketSize];
    uint8_t _prevContinuityCount;
};

} // namespace Media
