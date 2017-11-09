#include "VideoStreamHandler.hpp"

using namespace Media;

VideoStreamHandler::VideoStreamHandler(IDecoder::Ptr decoder)
    : _pesPacket(decoder)
{
}

bool VideoStreamHandler::Parse(ByteIterator start, ByteIterator end, bool hasStartIndicator)
{
    return _pesPacket.Parse(start, end, hasStartIndicator);
}
