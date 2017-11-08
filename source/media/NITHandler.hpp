#pragma once

#include "media/IPIDDataHandler.hpp"

namespace Media
{

class IStreamCallback;

class NITHandler : public IPIDDataHandler
{
public:
    NITHandler() {}

    bool Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator) override
    {
        throw std::logic_error("NIT not implemented");
    }

private:
};

} // namespace Media
