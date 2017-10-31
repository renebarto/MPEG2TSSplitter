#include "PEStream.hpp"

using namespace Media;

PEStream::PEStream(std::ostream & stream)
    : _stream(stream)
{

}

void PEStream::WritePESPacket(const uint8_t * data, size_t size)
{

}

