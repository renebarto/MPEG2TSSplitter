#include "MPEG2Demultiplexor.hpp"

#include <fstream>
#include <iostream>
#include "TSPacket.hpp"
#include "TransportStream.hpp"
#include "PEStream.hpp"

using namespace std;
using namespace Media;

MPEG2Demultiplexor::MPEG2Demultiplexor(const std::string & inputPath)
    : _inputPath(inputPath)
{
}

int MPEG2Demultiplexor::Run()
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
    PEStream audioStream(audioStreamFile);
    PEStream videoStream(videoStreamFile);
    bool havePacket = transportStream.ReadPacket(transportStreamPacket);
    while (havePacket)
    {
        cout << "Read packet " << transportStream.PacketIndex() << endl;
        if (transportStreamPacket.IsAudio())
        {
            audioStream.WritePESPacket(transportStreamPacket.Data(), transportStreamPacket.Size());
        }
        if (transportStreamPacket.IsVideo())
        {
            videoStream.WritePESPacket(transportStreamPacket.Data(), transportStreamPacket.Size());
        }
        havePacket = transportStream.ReadPacket(transportStreamPacket);
    }

    return 0;
}
