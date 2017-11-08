#pragma once

#include "media/IPIDDataHandler.hpp"

namespace Media
{

class IStreamCallback;

class CATHandler : public IPIDDataHandler
{
public:
    CATHandler() {}

    bool Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator) override
    {
        throw std::logic_error("CAT not implemented");
    }

private:
};

} // namespace Media
