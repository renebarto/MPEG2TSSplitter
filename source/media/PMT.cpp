#include "media/PMT.hpp"

#include<iostream>
#include<iomanip>
#include "tools/SerializationImpl.hpp"
#include "tools/Console.hpp"
#include "media/IStreamCallback.hpp"

using namespace std;
using namespace Media;

PMT::PMT(IStreamCallback * streamInfoCallback)
    : _streamInfoCallback(streamInfoCallback)
    , _programNumber()
    , _pcrPID()
    , _programInfoLength()
    , _streamInfo()
{

}

bool PMT::Parse(std::vector<uint8_t> & data)
{
    Tools::BitBuffer buffer(data.begin(), data.end());
    _tableID = static_cast<TableID>(buffer.ReadBits(8));
    if (_tableID != TableID::PMT)
        return false;
    if (!buffer.ReadBit())  // section_syntax_indicator
        return false;
    if (buffer.ReadBit())   // '0'
        return false;
    buffer.ReadBits(2);     // Reserved xx
    _sectionLength = static_cast<uint16_t>(buffer.ReadBits(12));
    _needMoreData = (buffer.BytesLeft() < _sectionLength);
    if (_needMoreData)
        return false;
    _programNumber = static_cast<uint16_t>(buffer.ReadBits(16));
    buffer.ReadBits(2);     // Reserved xx
    _versionNumber = static_cast<uint8_t>(buffer.ReadBits(5));
    _current = buffer.ReadBit();
    _sectionNumber = static_cast<uint8_t>(buffer.ReadBits(8));
    _lastSectionNumber = static_cast<uint8_t>(buffer.ReadBits(8));
    buffer.ReadBits(3);     // Reserved xxx
    _pcrPID = static_cast<PIDType>(buffer.ReadBits(13));
    buffer.ReadBits(4);     // Reserved xxxx
    _programInfoLength = static_cast<uint16_t>(buffer.ReadBits(12));
    size_t programInfoStartOffset = buffer.ByteOffset();
    while (buffer.ByteOffset() - programInfoStartOffset < _programInfoLength)
    {
        if (!ParseDescriptor(buffer, _streamInfo.programDescriptors))
            return false;
    }
    while (buffer.BytesLeft() > 4)
    {
        if (!ParseStreamInfo(buffer))
            return false;
    }
    _crc = static_cast<uint32_t>(buffer.ReadBits(32));
    return IsValid();
}

bool PMT::ParseDescriptor(Tools::BitBuffer & buffer, ProgramDescriptorList & list)
{
    auto descriptor = std::make_shared<ProgramDescriptor>();
    if (!descriptor->Parse(buffer))
        return false;
    list.push_back(descriptor);
    return true;
}

bool PMT::ParseStreamInfo(Tools::BitBuffer & buffer)
{
    StreamInfo info;
    if (!info.Parse(buffer))
        return false;
    _streamInfo.streamInfo.push_back(info);
    if (_streamInfoCallback)
        _streamInfoCallback->OnStreamFound(PIDKind::PES, info);
    return true;
}

bool PMT::IsValid() const
{
    return (_tableID == TableID::PMT) &&
           ((_sectionLength & 0xFC00) == 0x0000) && // section_length = 0000 00xx xxxx xxxx
           (_sectionNumber == 0x00) &&
           (_lastSectionNumber == 0x00) &&
           ((_programInfoLength & 0xFC00) == 0x0000); // program_info_length = 0000 00xx xxxx xxxx;
}

bool PMT::HaveAudio() const
{
    for (auto & streamInfo : _streamInfo.streamInfo)
    {
        if (streamInfo.IsAudioStream())
            return true;
    }
    return false;
}

bool PMT::HaveVideo() const
{
    for (auto & streamInfo : _streamInfo.streamInfo)
    {
        if (streamInfo.IsVideoStream())
            return true;
    }
    return false;
}

PIDType PMT::GetAudioPID() const
{
    for (auto & streamInfo : _streamInfo.streamInfo)
    {
        if (streamInfo.IsAudioStream())
            return streamInfo.PID();
    }
    return PIDType::NULL_PACKET;
}

PIDType PMT::GetVideoPID() const
{
    for (auto & streamInfo : _streamInfo.streamInfo)
    {
        if (streamInfo.IsVideoStream())
            return streamInfo.PID();
    }
    return PIDType::NULL_PACKET;
}

void PMT::DisplayContents() const
{
    Tools::DefaultConsole() << endl << "PMT" << endl << endl;
    Tools::DefaultConsole() << "TableID:          " << _tableID << endl;
    Tools::DefaultConsole() << "Program number:   " << Tools::Serialize(_programNumber, 16) << endl;
    Tools::DefaultConsole() << "Version:          " << Tools::Serialize(_versionNumber, 16)
                            << " (" << (_current ? "CURRENT" : "NEXT") << ")" << endl;
    Tools::DefaultConsole() << "Sections:         " << Tools::Serialize(_sectionNumber) << " - "
         << Tools::Serialize(_lastSectionNumber) << endl;
    for (auto & programInfo : _streamInfo.programDescriptors)
    {
        programInfo->DisplayContents();
    }
    for (auto & streamInfo : _streamInfo.streamInfo)
    {
        streamInfo.DisplayContents();
    }
}

