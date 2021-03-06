#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <tools/Definitions.hpp>
#include <tools/BitBuffer.hpp>

namespace Media
{

class IDecoder
{
public:
    using Ptr = std::shared_ptr<IDecoder>;

    IDecoder() = default;
    virtual ~IDecoder() = default;

    virtual void FeedData(ByteIterator data, size_t size) = 0;
};

} // namespace Media