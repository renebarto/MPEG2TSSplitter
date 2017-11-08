#pragma once

#include "media/IPIDDataHandler.hpp"

namespace Media
{

class IStreamCallback;

class ICITHandler : public IPIDDataHandler
{
public:
    ICITHandler() {}

    bool Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator) override
    {
        throw std::logic_error("ICIT not implemented");
    }

private:
};

} // namespace Media
