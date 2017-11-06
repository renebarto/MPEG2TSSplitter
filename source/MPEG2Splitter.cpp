#include "MPEG2Splitter.hpp"

#include <fstream>
#include <iostream>
#include "TSPacket.hpp"
#include "TransportStream.hpp"
#include "PEStream.hpp"

using namespace std;
using namespace Media;

MPEG2Splitter::MPEG2Splitter(const std::string & inputPath)
    : _inputPath(inputPath)
    , _audioPID(PIDType::NULL_PACKET)
    , _videoPID(PIDType::NULL_PACKET)
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

int MPEG2Splitter::Run()
{
    TSPacket transportStreamPacket;
    ifstream transportStreamFile(_inputPath);
    if (!transportStreamFile)
    {
        cerr << "Could not open input stream " << _inputPath << endl;
        return EXIT_FAILURE;
    }
    ofstream audioStreamFile;
    ofstream videoStreamFile;
    TransportStream transportStream(transportStreamFile);
    transportStream.SetAudioPID(_audioPID);
    transportStream.SetVideoPID(_videoPID);

//    PEStream audioStream(audioStreamFile);
//    PEStream videoStream(videoStreamFile);
    bool havePacket = transportStream.ReadPacket(transportStreamPacket);
    while (havePacket)
    {
//        if (transportStreamPacket.IsAudio())
//        {
//            audioStream.WritePESPacket(transportStreamPacket.Data(), transportStreamPacket.PayloadSize());
//        }
//        if (transportStreamPacket.IsVideo())
//        {
//            videoStream.WritePESPacket(transportStreamPacket.Data(), transportStreamPacket.PayloadSize());
//        }
        havePacket = transportStream.ReadPacket(transportStreamPacket);
    }

    return 0;
}
