#include "media/PMT.hpp"

#include "tools/SerializationImpl.hpp"
#include "tools/Console.hpp"
#include "media/IStreamCallback.hpp"

using namespace std;
using namespace Media;

PMT::PMT(IStreamCallback * streamInfoCallback)
    : _streamInfoCallback(streamInfoCallback)
    , _tableID()
    , _sectionLength()
    , _needMoreData(true)
    , _versionNumber()
    , _current()
    , _sectionNumber()
    , _lastSectionNumber()
    , _crc()
    , _programNumber()
    , _pcrPID()
    , _programInfoLength()
    , _streamInfo()
{

}

bool PMT::Parse(ByteIterator start, ByteIterator end)
{
    Tools::BitBuffer buffer(start, end);
    _tableID = static_cast<TableID>(buffer.ReadBits<uint8_t>(8));
    if (_tableID != TableID::PMT)
        return false;
    if (!buffer.ReadBit())  // section_syntax_indicator
        return false;
    if (buffer.ReadBit())   // '0'
        return false;
    buffer.ReadBits<uint8_t>(2);     // Reserved xx
    _sectionLength = buffer.ReadBits<uint16_t>(12);
    _needMoreData = (buffer.BytesLeft() < _sectionLength);
    if (_needMoreData)
        return false;
    _programNumber = buffer.ReadBits<uint16_t>(16);
    buffer.ReadBits<uint8_t>(2);     // Reserved xx
    _versionNumber = buffer.ReadBits<uint8_t>(5);
    _current = buffer.ReadBit();
    _sectionNumber = buffer.ReadBits<uint8_t>(8);
    _lastSectionNumber = buffer.ReadBits<uint8_t>(8);
    buffer.ReadBits<uint8_t>(3);     // Reserved xxx
    _pcrPID = static_cast<PIDType>(buffer.ReadBits<uint16_t>(13));
    buffer.ReadBits<uint8_t>(4);     // Reserved xxxx
    _programInfoLength = buffer.ReadBits<uint16_t>(12);
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
    _crc = buffer.ReadBits<uint32_t>(32);
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

