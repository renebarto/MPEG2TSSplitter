#include "VideoStreamHandler.hpp"

using namespace Media;

VideoStreamHandler::VideoStreamHandler(IDecoder::Ptr decoder, IStreamCallback * streamInfoCallback, bool verbose)
    : _pesPacket(decoder)
    , _verbose(verbose)
{

}

bool VideoStreamHandler::Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator)
{
    return _pesPacket.Parse(start, end, hasStartIndicator);
}
