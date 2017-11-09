#pragma once

#include <cstdint>
#include "media/IDecoder.hpp"
#include "media/IPIDDataHandler.hpp"
#include "media/PAT.hpp"

namespace Media
{

class IStreamCallback;
class IDecoder;

class PATHandler : public IPIDDataHandler
{
public:
    PATHandler(IStreamCallback * streamInfoCallback);

    bool Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator) override;

private:
    IStreamCallback * _streamInfoCallback;
    std::vector<uint8_t> _buffer;
    PAT _PAT; // Program Association Table
};

} // namespace Media
