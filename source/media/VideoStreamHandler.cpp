#include "VideoStreamHandler.hpp"

using namespace Media;

VideoStreamHandler::VideoStreamHandler()
    : _buffer()
      , _pesPacket()
{

}

bool VideoStreamHandler::Parse(const TSBufferIterator & start, const TSBufferIterator & end, bool hasStartIndicator)
{
    _buffer.insert(_buffer.end(), start, end);
    if (!_pesPacket.Parse(_buffer, hasStartIndicator))
        return false;
    _buffer.clear();
    return true;
}
