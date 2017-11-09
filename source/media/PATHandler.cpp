#include "PATHandler.hpp"

#include "Logging.hpp"

using namespace Media;

PATHandler::PATHandler(IStreamCallback * streamInfoCallback)
    : _streamInfoCallback(streamInfoCallback)
    , _buffer()
    , _PAT(streamInfoCallback)
{

}

bool PATHandler::Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator)
{
    if (hasStartIndicator)
    {
        uint8_t pointerField = *start;
        start += 1 + pointerField;
    }
    while ((end >= start) && (*(end - 1) == 0xFF))
        --end;

    _buffer.insert(_buffer.end(), start, end);
    if (_PAT.Parse(_buffer.begin(), _buffer.end()))
    {
        if (VerboseLogging())
            _PAT.DisplayContents();
        _buffer.clear();
    }
    else if (!_PAT.NeedMoreData())
        return false;
    return true;
}
