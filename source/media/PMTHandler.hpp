#pragma once

#include <cstdint>
#include "media/IPIDDataHandler.hpp"
#include "media/PMT.hpp"

namespace Media
{

class IStreamCallback;

class PMTHandler : public IPIDDataHandler
{
public:
    PMTHandler(IStreamCallback * streamInfoCallback);

    bool Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator) override;

private:
    IStreamCallback * _streamInfoCallback;
    std::vector<uint8_t> _buffer;
    PMT _PMT; // Program Map Table
};

} // namespace Media
