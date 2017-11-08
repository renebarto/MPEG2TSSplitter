#include "PMTHandler.hpp"

using namespace Media;

PMTHandler::PMTHandler(IStreamCallback * streamInfoCallback, bool verbose)
    : _streamInfoCallback(streamInfoCallback)
    , _verbose(verbose)
    , _buffer()
    , _PMT(streamInfoCallback)
{

}

bool PMTHandler::Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator)
{
    if (hasStartIndicator)
    {
        uint8_t pointerField = *start;
        start += 1 + pointerField;
    }
    while ((end >= start) && (*(end - 1) == 0xFF))
        --end;

    _buffer.insert(_buffer.end(), start, end);
    if (_PMT.Parse(_buffer))
    {
        if (_verbose)
            _PMT.DisplayContents();
        _buffer.clear();
    }
    else if (!_PMT.NeedMoreData())
        return false;
    return true;
}
