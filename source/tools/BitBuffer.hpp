#pragma once

#include <cstddef>
#include "tools/Definitions.hpp"

namespace Tools
{

constexpr size_t BITS_PER_BYTE = 8;

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

    template<typename T>
    T ReadBits(size_t count)
    {
        size_t bitsLeft = count;
        T result {};
        while ((bitsLeft > 0) && (_bitIndex != 0))
        {
            result = (result << 1) | ReadBit();
            --bitsLeft;
        }
        while ((bitsLeft >= 8) && (_byteIndex < _size))
        {
            result = (result << 8) | _begin[_byteIndex];
            ++_byteIndex;
            bitsLeft -= 8;
        }
        while (bitsLeft > 0)
        {
            result = (result << 1) | ReadBit();
            --bitsLeft;
        }
        return result;
    }

    template<typename T>
    T ReadAheadBits(size_t count)
    {
        size_t byteIndexSaved = _byteIndex;
        uint8_t bitIndexSaved = _bitIndex;
        try
        {
            T result = ReadBits<T>(count);
            _byteIndex = byteIndexSaved;
            _bitIndex = bitIndexSaved;
            return result;
        }
        catch (...)
        {
            _byteIndex = byteIndexSaved;
            _bitIndex = bitIndexSaved;
            throw;
        }
    }

//    uint64_t ReadBits(size_t count);
//    uint64_t ReadAheadBits(size_t count);
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
