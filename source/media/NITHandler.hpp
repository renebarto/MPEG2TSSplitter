#pragma once

#include "media/IPIDDataHandler.hpp"

namespace Media
{

class IStreamCallback;

class NITHandler : public IPIDDataHandler
{
public:
    NITHandler() {}

    bool Parse(ByteIterator start PARAMETER_NOT_USED, ByteIterator end PARAMETER_NOT_USED,
               bool hasStartIndicator PARAMETER_NOT_USED) override
    {
        throw std::logic_error("NIT not implemented");
    }

private:
};

} // namespace Media
