#include "AudioStreamHandler.hpp"

using namespace Media;

AudioStreamHandler::AudioStreamHandler(IDecoder::Ptr decoder, IStreamCallback * streamInfoCallback, bool verbose)
    : _pesPacket(decoder)
    , _verbose(verbose)
{

}

bool AudioStreamHandler::Parse(Tools::ByteIterator start, Tools::ByteIterator end, bool hasStartIndicator)
{
    return _pesPacket.Parse(start, end, hasStartIndicator);
}
