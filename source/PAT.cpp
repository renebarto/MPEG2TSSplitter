#include "PAT.hpp"

#include<sstream>

using namespace Media;

PAT::PAT()
    : _tableID()
    , _sectionLength()
    , _pidPMT()
    , _pidNIT()
{

}

bool PAT::Parse(std::deque<uint8_t> & buffer)
{
    _tableID = static_cast<TableID>(buffer[0]);
    // section_syntax_indicator = 1, next bit = 0, skip two reserved bits
    // 10xx
    if ((buffer[1] & 0xC0) != 0xC0)
        return false;
    _sectionLength = static_cast<uint16_t>((buffer[1] & 0x0F) << 8 | buffer[2]);
    if (buffer.size() < _sectionLength + 3)
        return false;
//    _transportStreamID =
    return IsValid();
}

bool PAT::IsValid() const
{
    return (_tableID == TableID::PAT) &&            // table_id = 0x00
           ((_sectionLength & 0x0C00) == 0x0000);   // section_length = 00xx xxxx xxxx
}

std::string PAT::ToString() const
{
    std::ostringstream stream;
    stream << _tableID;
    return stream.str();
}
