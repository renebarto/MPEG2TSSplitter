#include "BitBuffer.hpp"

#include <cassert>
#include <stdexcept>

using namespace Tools;

BitBuffer::BitBuffer()
    : _data()
    , _byteIndex()
    , _bitIndex()
{

}

void BitBuffer::SetData(const std::deque<uint8_t> & data)
{
    _data.reserve(data.size());
    std::copy(data.begin(), data.end(), std::back_inserter(_data));
}

void BitBuffer::SetData(const std::vector<uint8_t> & data)
{
    _data = data;
    _byteIndex = _bitIndex = 0;
}

size_t BitBuffer::BytesLeft() const
{
    return _data.size() - _byteIndex - ((_bitIndex == 0) ? 0 : 1);
}

size_t BitBuffer::BitsLeft() const
{
    return BytesLeft() * BITS_PER_BYTE + ((_bitIndex == 0) ? 0 : BITS_PER_BYTE - _bitIndex);
}

void BitBuffer::Reset()
{
    _byteIndex = _bitIndex = 0;
}

uint8_t BitBuffer::ReadBit()
{
    if ((_byteIndex >= _data.size()) || (_bitIndex >= BITS_PER_BYTE))
        throw std::runtime_error("Reading beyond BitBuffer");
    uint8_t result = static_cast<uint8_t>((_data[_byteIndex] >> (BITS_PER_BYTE - _bitIndex - 1)) & 0x01);
    ++_bitIndex;
    if (_bitIndex >= BITS_PER_BYTE)
    {
        _bitIndex = 0;
        ++_byteIndex;
    }
    return result;
}

uint64_t BitBuffer::ReadBits(size_t count)
{
    if (count > 64)
        throw std::runtime_error("Read exceeding 64 bits");
    size_t bitsLeft = count;
    uint64_t result {};
    while ((bitsLeft > 0) && (_bitIndex != 0))
    {
        result = (result << 1) | ReadBit();
        --bitsLeft;
    }
    while ((bitsLeft >= 8) && (_byteIndex < _data.size()))
    {
        result = (result << 8) | _data[_byteIndex];
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

uint64_t BitBuffer::ReadAheadBits(size_t count)
{
    size_t byteIndexSaved = _byteIndex;
    uint8_t bitIndexSaved = _bitIndex;
    try
    {
        uint64_t result = ReadBits(count);
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

void BitBuffer::SkipBytes(size_t count)
{
    if (_byteIndex + count >= _data.size())
        throw std::runtime_error("Reading beyond BitBuffer");
    _byteIndex += count;
}
