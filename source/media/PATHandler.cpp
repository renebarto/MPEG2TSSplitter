#include "PATHandler.hpp"

using namespace Media;

PATHandler::PATHandler(IStreamCallback * streamInfoCallback, bool verbose)
    : _streamInfoCallback(streamInfoCallback)
    , _verbose(verbose)
    , _buffer()
    , _PAT(streamInfoCallback)
{

}

bool PATHandler::Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator)
{
    if (hasStartIndicator)
    {
        uint8_t pointerField = *start;
        start += 1 + pointerField;
    }
    while ((end >= start) && (*(end - 1) == 0xFF))
        --end;

    _buffer.insert(_buffer.end(), start, end);
    if (_PAT.Parse(_buffer))
    {
        if (_verbose)
            _PAT.DisplayContents();
        _buffer.clear();
    }
    else if (!_PAT.NeedMoreData())
        return false;
    return true;
}
