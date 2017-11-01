#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Media
{

constexpr size_t BITS_PER_BYTE = 8;

class BitBuffer
{
public:
    BitBuffer();

    void SetData(const std::vector<uint8_t> & data);

    size_t BytesLeft() const;
    size_t BitsLeft() const;

    uint8_t ReadBit();
    uint64_t ReadBits(size_t count);

private:
    std::vector<uint8_t> _data;
    size_t _byteIndex;
    uint8_t _bitIndex;
};

} // namespace Media
