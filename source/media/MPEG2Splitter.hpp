#pragma once

#include <string>
#include <fstream>
#include "media/TSDefinitions.hpp"
#include "media/IStreamCallback.hpp"
#include "media/IDecoder.hpp"
#include "media/TransportStream.hpp"

namespace Media
{

class MPEG2Splitter : public IStreamCallback
{
public:
    MPEG2Splitter() = delete;
    MPEG2Splitter(const MPEG2Splitter &) = delete;
    MPEG2Splitter & operator= (const MPEG2Splitter &) = delete;

    MPEG2Splitter(const std::string & inputPath);

    void SetVerbose(bool value);
    void SetAudioPID(PIDType pid);
    void SetVideoPID(PIDType pid);
    IDecoder::Ptr CreateAudioDecoder(StreamType type);
    IDecoder::Ptr CreateVideoDecoder(StreamType type);

    int Run();

    virtual void OnStreamFound(const StreamInfo & info) override;

private:
    std::string _inputPath;
    PIDType _audioPID;
    PIDType _videoPID;
    std::ifstream _transportStreamFile;
    std::ofstream _audioStream;
    std::ofstream _videoStream;
    TransportStream _transportStream;
    IDecoder::Ptr _audioDecoder;
    IDecoder::Ptr _videoDecoder;
};

} // namespace Media
