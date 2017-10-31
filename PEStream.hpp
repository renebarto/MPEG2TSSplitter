#pragma once

#include <iostream>

namespace Media
{

class PEStream
{
public:
    PEStream() = delete;
    PEStream(const PEStream &) = delete;
    PEStream & operator= (const PEStream &) = delete;

    explicit PEStream(std::ostream & stream);

    void WritePESPacket(const uint8_t * data, size_t size);

private:
    std::ostream & _stream;
};

} // namespace Media