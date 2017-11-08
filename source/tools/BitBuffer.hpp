#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <deque>

namespace Tools
{

constexpr size_t BITS_PER_BYTE = 8;

using ByteIterator = std::vector<uint8_t>::const_iterator;

class BitBuffer
{
public:
    BitBuffer(ByteIterator start, ByteIterator end);

    size_t Size() const;
    size_t BytesLeft() const;
    size_t BitsLeft() const;

    ByteIterator Begin() const { return _begin; }
    ByteIterator End() const { return _begin + _size; }
    void Reset();
    uint8_t ReadBit();
    uint64_t ReadBits(size_t count);
    uint64_t ReadAheadBits(size_t count);
    size_t ByteOffset() { return _byteIndex; }
    ByteIterator Current() { return Begin() +_byteIndex; }
    void SkipBytes(size_t count);

private:
    ByteIterator _begin;
    size_t _size;
    size_t _byteIndex;
    uint8_t _bitIndex;
};

} // namespace Tools
