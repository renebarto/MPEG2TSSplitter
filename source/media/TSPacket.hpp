#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include "tools/BitBuffer.hpp"
#include "media/TSDefinitions.hpp"

namespace Media
{

enum class AdaptationFieldControlType : uint8_t
{
    Reserved = 0x0,
    PayloadOnly = 0x1,
    AdaptationOnly = 0x2,
    AdaptationAndPayload = 0x3,
};

class TSPacket
{
public:
    static constexpr size_t PacketSize = 188;

    TSPacket();
    TSPacket(const TSPacket &) = delete;
    TSPacket & operator= (const TSPacket &) = delete;

    bool Parse(const std::vector<uint8_t> & buffer);

    const std::vector<uint8_t> & Data() const { return _data.Data(); }

    bool IsValid() const;
    bool IsAudio() const { return false; }
    bool IsVideo() const { return false; }

    uint32_t PacketHeader() const { return _packetHeader; }
    bool HasError() const;
    bool HasPayloadUnitStartIndicator() const;
    bool HasTransportPriority() const;
    PIDType PID() const;
    ScrambingControl TransportScramblingControl() const;
    AdaptationFieldControlType AdaptationFieldControl() const;
    uint8_t ContinuityCount() const;
    uint8_t AdaptationFieldSize() const;
    uint8_t PayloadSize() const;
    bool HasAdaptationField() const;
    bool HasPayload() const;
    bool HasDiscontinuity() const;
    bool HasCounterDiscontinuity() const;
    bool IsDuplicate() const;
    uint8_t PayloadStartOffset() const;

    void DisplayContents(size_t packetIndex) const;

private:
    Tools::BitBuffer _data;
    uint32_t _packetHeader;
    bool _transportErrorIndicator;
    bool _payloadUnitStartIndicator;
    bool _transportPriority;
    PIDType _pid;
    ScrambingControl _scramblingControl;
    AdaptationFieldControlType _adaptationFieldControl;
    uint8_t _continuityCount;
    uint8_t _prevContinuityCount;

    // Adaptation Field
    uint8_t _adaptationFieldSize;
    bool _discontinuityIndicator;

    bool ParseAdaptationField();
};

} // namespace Media
