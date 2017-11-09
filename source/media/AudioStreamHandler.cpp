#include "AudioStreamHandler.hpp"

using namespace Media;

AudioStreamHandler::AudioStreamHandler(IDecoder::Ptr decoder)
    : _pesPacket(decoder)
{

}

bool AudioStreamHandler::Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator)
{
    return _pesPacket.Parse(start, end, hasStartIndicator);
}
