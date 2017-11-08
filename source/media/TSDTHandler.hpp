#pragma once

#include "media/IPIDDataHandler.hpp"

namespace Media
{

class IStreamCallback;

class TSDTHandler : public IPIDDataHandler
{
public:
    TSDTHandler() {}

    bool Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator) override
    {
        throw std::logic_error("TSDT not implemented");
    }

private:
};

} // namespace Media
