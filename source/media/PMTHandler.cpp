#include "PMTHandler.hpp"

#include "Logging.hpp"

using namespace Media;

PMTHandler::PMTHandler(IStreamCallback * streamInfoCallback)
    : _streamInfoCallback(streamInfoCallback)
    , _buffer()
    , _PMT(streamInfoCallback)
{

}

bool PMTHandler::Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator)
{
    if (hasStartIndicator)
    {
        uint8_t pointerField = *start;
        start += 1 + pointerField;
    }
    while ((end >= start) && (*(end - 1) == 0xFF))
        --end;

    _buffer.insert(_buffer.end(), start, end);
    if (_PMT.Parse(_buffer.begin(), _buffer.end()))
    {
        if (VerboseLogging())
            _PMT.DisplayContents();
        _buffer.clear();
    }
    else if (!_PMT.NeedMoreData())
        return false;
    return true;
}
