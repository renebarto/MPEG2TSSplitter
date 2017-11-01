#include "BitBuffer.hpp"

#include <cassert>

using namespace Media;

BitBuffer::BitBuffer()
    : _data()
    , _byteIndex()
    , _bitIndex()

{

}

void BitBuffer::SetData(const std::vector<uint8_t> & data)
{
    _data = data;
}

size_t BitBuffer::BytesLeft() const
{
    return _data.size() - _byteIndex - ((_bitIndex == 0) ? 0 : 1);
}

size_t BitBuffer::BitsLeft() const
{
    return BytesLeft() * BITS_PER_BYTE + ((_bitIndex == 0) ? 0 : BITS_PER_BYTE - _bitIndex);
}

uint8_t BitBuffer::ReadBit()
{
    assert(_byteIndex < _data.size());
    assert(_bitIndex < BITS_PER_BYTE);
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
    assert(count <= 64);
    size_t bitsLeft = count;
    uint64_t result {};
    while (_bitIndex != 0)
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
