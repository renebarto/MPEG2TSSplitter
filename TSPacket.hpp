#pragma once

#include <cstddef>
#include <cstdint>

namespace Media
{

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

    const uint8_t * Data() const { return _data; }
    uint8_t * Data() { return _data; }
    size_t Size() const { return 0; }
    ESType StreamType() { return ESType::Unknown; }

    bool IsValid() const;
    bool IsAudio() const { return false; }
    bool IsVideo() const { return false; }

    uint32_t Prefix() const { return *reinterpret_cast<const uint32_t *>(_data); }
    uint16_t PID() const { return 0; }

private:
    uint8_t _data[PacketSize];

    int _PAT; // Program Association Table
    int _PMT; // Program Map Table
    int _CAT; // Conditional Access Table
    int _NIT; // Network Information Table
};

} // namespace Media
