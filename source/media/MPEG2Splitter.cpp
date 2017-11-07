#include "media/MPEG2Splitter.hpp"

#include <fstream>
#include <iostream>
#include "tools/Console.hpp"
#include "tools/SerializationImpl.hpp"
#include "media/TSPacket.hpp"
#include "media/TransportStream.hpp"
#include "media/StreamWriter.hpp"

using namespace std;
using namespace Media;
using namespace Tools;

MPEG2Splitter::MPEG2Splitter(const std::string & inputPath)
    : _inputPath(inputPath)
    , _audioPID(PIDType::NULL_PACKET)
    , _videoPID(PIDType::NULL_PACKET)
    , _transportStreamFile()
    , _audioStream()
    , _videoStream()
    , _transportStream(_transportStreamFile, this)
{
}

void MPEG2Splitter::SetVerbose(bool value)
{
    _transportStream.SetVerbose(value);
}

void MPEG2Splitter::SetAudioPID(PIDType pid)
{
    _audioPID = pid;
}

void MPEG2Splitter::SetVideoPID(PIDType pid)
{
    _videoPID = pid;
}

void MPEG2Splitter::OnStreamFound(const StreamInfo & info)
{
    DefaultConsole() << fgcolor(ConsoleColor::Cyan) << "Stream Callback stream type: " << info.Type()
                     << " pid: " << Serialize(static_cast<uint16_t>(info.PID()), 16) << fgcolor(ConsoleColor::Default) << endl;
    if ((_audioPID == PIDType::NULL_PACKET) && info.IsAudioStream())
    {
        _audioPID = info.PID();
        _audioDecoder = CreateAudioDecoder(info.Type());
        _transportStream.SetAudioPID(_audioPID, _audioDecoder);
        Tools::DefaultConsole() << "Using audio PID " << hex << setw(4) << setfill('0') << static_cast<uint16_t>(_audioPID)
                                << dec << endl;
    }
    if ((_videoPID == PIDType::NULL_PACKET) && info.IsVideoStream())
    {
        _videoPID = info.PID();
        _videoDecoder = CreateVideoDecoder(info.Type());
        _transportStream.SetVideoPID(_videoPID, _videoDecoder);
        Tools::DefaultConsole() << "Using video PID " << hex << setw(4) << setfill('0') << static_cast<uint16_t>(_videoPID)
                                << dec << endl;
    }
}

IDecoder::Ptr MPEG2Splitter::CreateAudioDecoder(StreamType type)
{
    switch (type)
    {
        case StreamType::AudioIEC13818_7:
        {
            string path = _inputPath + ".aac";
            _audioStream.open(path, ios_base::out | ios_base::trunc | ios_base::binary);
            return std::make_shared<StreamWriter>(_audioStream);
        }
        default:
            throw std::logic_error("Not implemented");
    }
    return nullptr;
}

IDecoder::Ptr MPEG2Splitter::CreateVideoDecoder(StreamType type)
{
    switch (type)
    {
        case StreamType::VideoIEC14496_10_AVC:
        {
            string path = _inputPath + ".mp4";
            _videoStream.open(path, ios_base::out | ios_base::trunc | ios_base::binary);
            return std::make_shared<StreamWriter>(_videoStream);
        }
        default:
            throw std::logic_error("Not implemented");
    }
    return nullptr;
}

int MPEG2Splitter::Run()
{
    TSPacket transportStreamPacket;
    _transportStreamFile.open(_inputPath);
    if (!_transportStreamFile)
    {
        cerr << "Could not open input stream " << _inputPath << endl;
        return EXIT_FAILURE;
    }
    _transportStream.SetAudioPID(_audioPID, nullptr);
    _transportStream.SetVideoPID(_videoPID, nullptr);

    bool havePacket = _transportStream.ReadPacket(transportStreamPacket);
    while (havePacket)
    {
        havePacket = _transportStream.ReadPacket(transportStreamPacket);
    }
    _audioStream.flush();
    _videoStream.flush();

    return 0;
}
