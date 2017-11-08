#include "media/PAT.hpp"

#include<iostream>
#include<iomanip>
#include "tools/SerializationImpl.hpp"
#include "tools/BitBuffer.hpp"
#include "tools/Console.hpp"
#include "media/IStreamCallback.hpp"
#include "media/StreamInfo.hpp"

using namespace std;
using namespace Media;

static constexpr PIDType DefaultPID_NIT = static_cast<PIDType>(0x0010);
PAT::PAT(IStreamCallback * streamInfoCallback)
    : _streamInfoCallback(streamInfoCallback)
    , _transportStreamID()
    , _programInfo()
{

}

bool PAT::Parse(std::vector<uint8_t> & data)
{
    Tools::BitBuffer buffer(data.begin(), data.end());
    _tableID = static_cast<TableID>(buffer.ReadBits(8));
    if (_tableID != TableID::PAT)
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
    _transportStreamID = static_cast<uint16_t>(buffer.ReadBits(16));
    buffer.ReadBits(2);     // Reserved xx
    _versionNumber = static_cast<uint8_t>(buffer.ReadBits(5));
    _current = buffer.ReadBit();
    _sectionNumber = static_cast<uint8_t>(buffer.ReadBits(8));
    _lastSectionNumber = static_cast<uint8_t>(buffer.ReadBits(8));
    for (uint8_t section = _sectionNumber; section <= _lastSectionNumber; ++section)
    {
        ProgramAssociation info {};
        info.programNumber = static_cast<uint16_t>(buffer.ReadBits(16));
        buffer.ReadBits(3);     // Reserved xxx
        info.pid = static_cast<PIDType>(buffer.ReadBits(13));
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
    _crc = static_cast<uint32_t>(buffer.ReadBits(32));
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
