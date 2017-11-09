#pragma once

#include <memory>
#include "tools/Definitions.hpp"
#include "tools/BitBuffer.hpp"
#include "media/PAT.hpp"

namespace Media
{

class IStreamCallback;
class IDecoder;

class IPIDDataHandler
{
public:
    using Ptr = std::shared_ptr<IPIDDataHandler>;

    virtual ~IPIDDataHandler() = default;

    virtual bool Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator) = 0;
};

} // namespace Media
