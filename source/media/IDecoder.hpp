#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace Media
{

class IDecoder
{
public:
    using Ptr = std::shared_ptr<IDecoder>;

    IDecoder() = default;
    virtual ~IDecoder() = default;

    virtual void FeedData(const uint8_t * data, size_t size) = 0;
};

} // namespace Media