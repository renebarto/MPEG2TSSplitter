#pragma once

#include "media/IPIDDataHandler.hpp"

namespace Media
{

class IStreamCallback;

class ICITHandler : public IPIDDataHandler
{
public:
    ICITHandler() {}

    bool Parse(ByteIterator start PARAMETER_NOT_USED, ByteIterator end PARAMETER_NOT_USED,
               bool hasStartIndicator PARAMETER_NOT_USED) override
    {
        throw std::logic_error("ICIT not implemented");
    }

private:
};

} // namespace Media
