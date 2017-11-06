#include "CommandLineOptionsParser.hpp"
#include "TSDefinitions.hpp"

CommandLineOptionsParser::CommandLineOptionsParser(Tools::Console & console)
    : Tools::CommandLineParser(console, "MPEG2 TS splitter", "Splits MPEG2 TS into audio and video elementary streams."
        "One file must be specified.\n"
        "This file must be a valid MPEG2 TS stream, which will be de-multiplexed into an elementary \n"
        "video and audio stream. Only the first audio and video stream will be extracted and saved into files.\n"
        "File extensions will be chosen such that the stream can be played in a media player, such as VLC.\n")
    , _inputPath()
    , _audioPID(static_cast<uint16_t>(Media::PIDType::NULL_PACKET))
    , _videoPID(static_cast<uint16_t>(Media::PIDType::NULL_PACKET))
{
    Tools::CommandLineOptionGroup::Ptr group =
        std::make_shared<Tools::CommandLineOptionGroup>("MPEG2 TS splitter", "Splits MPEG2 TS into audio and video elementary streams.");
    group->AddOptionRequiredArgumentWithVariable("input", 'i',
                                                 "MPEG2 Transport Stream path (must be specified)",
                                                 _inputPath);
    group->AddOptionRequiredArgumentWithVariable("audio-pid", 'a',
        "Pre-define audio PID to be used for extracting audio (default = first audio PID found)",
        _audioPID);
    group->AddOptionRequiredArgumentWithVariable("video-pid", 'v',
        "Pre-define video PID to be used for extracting video (default = first video PID found)",
        _videoPID);
    AddGroup(group);
}

