#include "media/PAT.hpp"

#include<iomanip>
#include "tools/SerializationImpl.hpp"
#include "tools/Console.hpp"
#include "media/IStreamCallback.hpp"
#include "media/StreamInfo.hpp"

using namespace std;
using namespace Media;

static constexpr PIDType DefaultPID_NIT = static_cast<PIDType>(0x0010);
PAT::PAT(IStreamCallback * streamInfoCallback)
    : _streamInfoCallback(streamInfoCallback)
    , _tableID()
    , _sectionLength()
    , _needMoreData(true)
    , _versionNumber()
    , _current()
    , _sectionNumber()
    , _lastSectionNumber()
    , _crc()
    , _transportStreamID()
    , _programInfo()
{

}

bool PAT::Parse(ByteIterator start, ByteIterator end)
{
    Tools::BitBuffer buffer(start, end);
    _tableID = static_cast<TableID>(buffer.ReadBits<uint8_t>(8));
    if (_tableID != TableID::PAT)
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
    _transportStreamID = buffer.ReadBits<uint16_t>(16);
    buffer.ReadBits<uint8_t>(2);     // Reserved xx
    _versionNumber = buffer.ReadBits<uint8_t>(5);
    _current = buffer.ReadBit();
    _sectionNumber = buffer.ReadBits<uint8_t>(8);
    _lastSectionNumber = buffer.ReadBits<uint8_t>(8);
    for (uint8_t section = _sectionNumber; section <= _lastSectionNumber; ++section)
    {
        ProgramAssociation info {};
        info.programNumber = buffer.ReadBits<uint16_t>(16);
        buffer.ReadBits<uint8_t>(3);     // Reserved xxx
        info.pid = static_cast<PIDType>(buffer.ReadBits<uint16_t>(13));
        _programInfo.push_back(info);
        if (_streamInfoCallback)
        {
            if (info.programNumber != 0x0000)
            {
                // Default NIT
                StreamInfo streamInfo(DefaultPID_NIT);
                _streamInfoCallback->OnStreamFound(PIDKind::NIT, streamInfo);
            }
            StreamInfo streamInfo(info.pid);
            _streamInfoCallback->OnStreamFound(PIDKind::PMT, streamInfo);
        }
    }
    _crc = buffer.ReadBits<uint32_t>(32);
    return IsValid();
}

bool PAT::IsValid() const
{
    return (_tableID == TableID::PAT) &&            // table_id = 0x00
           ((_sectionLength & 0xFC00) == 0x0000) && // section_length = 0000 00xx xxxx xxxx
           (_sectionNumber == 0x00);
}

void PAT::DisplayContents() const
{
    Tools::DefaultConsole() << endl << "PAT" << endl << endl;
    Tools::DefaultConsole() << "TableID:          " << _tableID << endl;
    Tools::DefaultConsole() << "TransportStreamID:" << Tools::Serialize(_transportStreamID, 16) << endl;
    Tools::DefaultConsole() << "Version:          " << Tools::Serialize(_versionNumber, 16)
                            << " (" << (_current ? "CURRENT" : "NEXT") << ")" << endl;
    Tools::DefaultConsole() << "Sections:         " << Tools::Serialize(_sectionNumber, 16) << " - "
                            << Tools::Serialize(_lastSectionNumber, 16) << endl;
    for (uint8_t section = _sectionNumber; section <= _lastSectionNumber; ++section)
    {
        Tools::DefaultConsole() << Tools::Serialize(section, 16)
         <<   ":               " << "Program " << Tools::Serialize(_programInfo[section].programNumber, 16)
                                 << " PID " << Tools::Serialize(static_cast<uint16_t>(_programInfo[section].pid), 16)
                                 << " (" << _programInfo[section].pid << ")" << endl;
    }
}
