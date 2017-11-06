#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <deque>

namespace Tools
{

constexpr size_t BITS_PER_BYTE = 8;

class BitBuffer
{
public:
    BitBuffer();

    void SetData(const std::deque<uint8_t> & data);
    void SetData(const std::vector<uint8_t> & data);

    size_t BytesLeft() const;
    size_t BitsLeft() const;

    const std::vector<uint8_t> & Data() const { return _data; }
    void Reset();
    uint8_t ReadBit();
    uint64_t ReadBits(size_t count);
    uint64_t ReadAheadBits(size_t count);
    size_t ByteOffset() { return _byteIndex; }
    void SkipBytes(size_t count);

private:
    std::vector<uint8_t> _data;
    size_t _byteIndex;
    uint8_t _bitIndex;
};

} // namespace Tools
