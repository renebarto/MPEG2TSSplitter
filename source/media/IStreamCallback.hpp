#pragma once

#include <cstddef>
#include <cstdint>

namespace Media
{

class StreamInfo;

class IStreamCallback
{
public:
    virtual ~IStreamCallback() {}

    virtual void OnStreamFound(const StreamInfo &) = 0;
};

} // namespace Media