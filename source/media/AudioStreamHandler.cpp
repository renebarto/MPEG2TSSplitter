#include "AudioStreamHandler.hpp"

using namespace Media;

AudioStreamHandler::AudioStreamHandler()
    : _buffer()
    , _pesPacket()
{

}

bool AudioStreamHandler::Parse(const TSBufferIterator & start, const TSBufferIterator & end, bool hasStartIndicator)
{
    _buffer.insert(_buffer.end(), start, end);
    if (!_pesPacket.Parse(_buffer, hasStartIndicator))
        return false;
    _buffer.clear();
    return true;
}
