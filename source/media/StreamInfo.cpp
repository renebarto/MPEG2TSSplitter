#include "media/StreamInfo.hpp"

#include "tools/SerializationImpl.hpp"
#include "tools/Console.hpp"

using namespace std;
using namespace Media;

StreamInfo::StreamInfo()
    : _type()
    , _pid()
    , _descriptors()
{
}

StreamInfo::StreamInfo(PIDType pid)
    : _type(StreamType::Reserved)
    , _pid(pid)
    , _descriptors()
{
}

bool StreamInfo::Parse(Tools::BitBuffer & buffer)
{
    _type = static_cast<StreamType>(buffer.ReadBits<uint8_t>(8));
    buffer.ReadBits<uint8_t>(3);     // Reserved xxx
    _pid = static_cast<PIDType>(buffer.ReadBits<uint16_t>(13));
    buffer.ReadBits<uint8_t>(4);     // Reserved xxxx
    uint16_t info_length = buffer.ReadBits<uint16_t>(12);
    size_t programInfoStartOffset = buffer.ByteOffset();
    while (buffer.ByteOffset() - programInfoStartOffset < info_length)
    {
        auto descriptor = std::make_shared<ProgramDescriptor>();
        if (!descriptor->Parse(buffer))
            return false;
        _descriptors.push_back(descriptor);
    }
    return true;
}

bool StreamInfo::IsValid() const
{
    return true;
}

bool StreamInfo::IsAudioStream() const
{
    switch (_type)
    {
        case StreamType::Reserved:
        case StreamType::VideoIEC11172:
        case StreamType::VideoIEC13818_2:
            return false;
        case StreamType::AudioIEC11172:
        case StreamType::AudioIEC13818_3:
            return true;
        case StreamType::PrivateSections:
        case StreamType::PESPrivateData:
        case StreamType::MHEG_IEC13522:
        case StreamType::DSM_CC:
        case StreamType::ITU_H222_1:
        case StreamType::IEC_13818_6A:
        case StreamType::IEC_13818_6B:
        case StreamType::IEC_13818_6C:
        case StreamType::IEC_13818_6D:
        case StreamType::AuxiliaryIEC13818_1:
            return false;
        case StreamType::AudioIEC13818_7:
            return true;
        case StreamType::VisualIEC14496_2:
            return false;
        case StreamType::AudioIEC14496_3:
            return true;
        case StreamType::IEC14496_1_PESPackets:
        case StreamType::IEC14496_1_IEC14496Sections:
        case StreamType::IEC13818_6_SynchronizedDownload:
        case StreamType::MetadataPESPackets:
        case StreamType::MetadataMetadataSections:
        case StreamType::MetadataIEC13838_6_DataCarousel:
        case StreamType::MetadataIEC13838_6_ObjectCarousel:
        case StreamType::MetadataIEC13838_6_SynchronizedDownload:
        case StreamType::IMPMPIEC13818_11:
        case StreamType::VideoIEC14496_10_AVC:
            return false;
        case StreamType::AudioIEC14496_3_NoTransportSyntax:
            return true;
        case StreamType::TextIEC14496_17:
            return false;
    }
    return false;
}

bool StreamInfo::IsVideoStream() const
{
    switch (_type)
    {
        case StreamType::Reserved:
            return false;
        case StreamType::VideoIEC11172:
        case StreamType::VideoIEC13818_2:
            return true;
        case StreamType::AudioIEC11172:
        case StreamType::AudioIEC13818_3:
        case StreamType::PrivateSections:
        case StreamType::PESPrivateData:
        case StreamType::MHEG_IEC13522:
        case StreamType::DSM_CC:
        case StreamType::ITU_H222_1:
        case StreamType::IEC_13818_6A:
        case StreamType::IEC_13818_6B:
        case StreamType::IEC_13818_6C:
        case StreamType::IEC_13818_6D:
        case StreamType::AuxiliaryIEC13818_1:
        case StreamType::AudioIEC13818_7:
        case StreamType::VisualIEC14496_2:
        case StreamType::AudioIEC14496_3:
        case StreamType::IEC14496_1_PESPackets:
        case StreamType::IEC14496_1_IEC14496Sections:
        case StreamType::IEC13818_6_SynchronizedDownload:
        case StreamType::MetadataPESPackets:
        case StreamType::MetadataMetadataSections:
        case StreamType::MetadataIEC13838_6_DataCarousel:
        case StreamType::MetadataIEC13838_6_ObjectCarousel:
        case StreamType::MetadataIEC13838_6_SynchronizedDownload:
        case StreamType::IMPMPIEC13818_11:
            return false;
        case StreamType::VideoIEC14496_10_AVC:
            return true;
        case StreamType::AudioIEC14496_3_NoTransportSyntax:
        case StreamType::TextIEC14496_17:
            return false;
    }
    return false;
}

void StreamInfo::DisplayContents() const
{
    Tools::DefaultConsole() << endl << "StreamInfo" << endl << endl;
    Tools::DefaultConsole() << "Stream type:      " << _type << endl;
    Tools::DefaultConsole() << "PID:              " << Tools::Serialize(static_cast<uint16_t>(_pid), 16) << endl;

    for (auto & programInfo : _descriptors)
    {
        programInfo->DisplayContents();
    }
}
