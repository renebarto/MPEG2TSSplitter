#pragma once

#include <cstddef>
#include <cstdint>
#include "media/TSDefinitions.hpp"

namespace Media
{

class StreamInfo;

class IStreamCallback
{
public:
    virtual ~IStreamCallback() {}

    virtual void OnStreamFound(PIDKind kind, const StreamInfo &) = 0;
};

} // namespace Media