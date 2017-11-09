#include "media/Logging.hpp"

namespace Media
{

static bool _verbose = false;

bool VerboseLogging()
{
    return _verbose;
}

void VerboseLogging(bool value)
{
    _verbose = value;
}

}
