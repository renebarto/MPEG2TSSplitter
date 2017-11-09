#include "tools/BitBuffer.hpp"

#include <stdexcept>

using namespace Tools;

BitBuffer::BitBuffer(ByteIterator start, ByteIterator end)
    : _begin(start)
    , _size(static_cast<size_t>(end - start))
    , _byteIndex()
    , _bitIndex()
{

}

size_t BitBuffer::Size() const
{
    return _size;
}

size_t BitBuffer::BytesLeft() const
{
    return _size - _byteIndex - ((_bitIndex == 0) ? 0 : 1);
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
    if (_byteIndex >= _size)
        throw std::runtime_error("Reading beyond BitBuffer");
    uint8_t result = static_cast<uint8_t>((_begin[_byteIndex] >> (BITS_PER_BYTE - _bitIndex - 1)) & 0x01);
    ++_bitIndex;
    if (_bitIndex >= BITS_PER_BYTE)
    {
        _bitIndex = 0;
        ++_byteIndex;
    }
    return result;
}

void BitBuffer::SkipBytes(size_t count)
{
    if (_byteIndex + count >= _size)
        throw std::runtime_error("Reading beyond BitBuffer");
    _byteIndex += count;
}
