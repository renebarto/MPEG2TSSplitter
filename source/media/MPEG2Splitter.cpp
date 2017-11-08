#include "media/MPEG2Splitter.hpp"

#include <fstream>
#include <iostream>
#include "tools/Console.hpp"
#include "tools/SerializationImpl.hpp"
#include "media/TSPacket.hpp"
#include "media/TransportStream.hpp"
#include "media/StreamWriter.hpp"
#include "NITHandler.hpp"

using namespace std;
using namespace Media;
using namespace Tools;

MPEG2Splitter::MPEG2Splitter(const std::string & inputPath, bool verbose)
    : _inputPath(inputPath)
    , _verbose(verbose)
    , _audioPID(PIDType::NULL_PACKET)
    , _videoPID(PIDType::NULL_PACKET)
    , _transportStreamFile()
    , _audioStream()
    , _videoStream()
    , _transportStream(_transportStreamFile, this, verbose)
{
}

void MPEG2Splitter::SetAudioPID(PIDType pid)
{
    _audioPID = pid;
}

void MPEG2Splitter::SetVideoPID(PIDType pid)
{
    _videoPID = pid;
}

void MPEG2Splitter::OnStreamFound(PIDKind kind, const StreamInfo & info)
{
    DefaultConsole() << fgcolor(ConsoleColor::Cyan) << "Stream Callback stream type: " << info.Type()
                     << " pid: " << Serialize(static_cast<uint16_t>(info.PID()), 16) << fgcolor(ConsoleColor::Default) << endl;
    if ((_audioPID == PIDType::NULL_PACKET) && info.IsAudioStream())
    {
        _audioPID = info.PID();
        _audioDecoder = CreateAudioDecoder(info.Type());
        auto handler = make_shared<AudioStreamHandler>(_audioDecoder, this, _verbose);
        _transportStream.AddStreamHandler(info.PID(), dynamic_pointer_cast<IPIDDataHandler>(handler));
        Tools::DefaultConsole() << "Using audio PID " << hex << setw(4) << setfill('0') << static_cast<uint16_t>(_audioPID)
                                << dec << endl;
    }
    else if ((_videoPID == PIDType::NULL_PACKET) && info.IsVideoStream())
    {
        _videoPID = info.PID();
        _videoDecoder = CreateVideoDecoder(info.Type());
        auto handler = make_shared<VideoStreamHandler>(_videoDecoder, this, _verbose);
        _transportStream.AddStreamHandler(info.PID(), dynamic_pointer_cast<IPIDDataHandler>(handler));
        Tools::DefaultConsole() << "Using video PID " << hex << setw(4) << setfill('0') << static_cast<uint16_t>(_videoPID)
                                << dec << endl;
    }
    else
    {
        switch (kind)
        {
            case PIDKind::PAT:      // PAT is already handled
                break;
            case PIDKind::CAT:      // CAT is already handled
                break;
            case PIDKind::TSDT:     // TSDT is already handled
                break;
            case PIDKind::ICIT:     // ICIT is already handled
                break;
            case PIDKind::PMT:
                _transportStream.AddStreamHandler(info.PID(), make_shared<PMTHandler>(this, _verbose));
                break;
            case PIDKind::NIT:
                _transportStream.AddStreamHandler(info.PID(), make_shared<NITHandler>());
                break;
            case PIDKind::PES:
                break;
        }
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
    _transportStream.Initialize();
    TSPacket tsPacket;
    _transportStreamFile.open(_inputPath);
    if (!_transportStreamFile.is_open())
    {
        cerr << "Could not open input stream " << _inputPath << endl;
        return EXIT_FAILURE;
    }

    bool havePacket = _transportStream.ReadPacket(tsPacket);
    while (havePacket)
    {
        _transportStream.ParsePacket(tsPacket);
        havePacket = _transportStream.ReadPacket(tsPacket);
    }
    _audioStream.flush();
    _videoStream.flush();

    return 0;
}
