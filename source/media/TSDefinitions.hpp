#pragma once

#include <iostream>

namespace Media
{

enum class TableID
{
    PAT = 0x00,
    CAT = 0x01,
    PMT = 0x02,
    TSDT = 0x03,
};

inline std::ostream & operator << (std::ostream & stream, TableID id)
{
    switch (id)
    {
        case TableID::PAT:
            stream << "PAT"; return stream;
        case TableID::CAT:
            stream << "CAT"; return stream;
        case TableID::PMT:
            stream << "PMT"; return stream;
        case TableID::TSDT:
            stream << "TSDT"; return stream;
    }
    return stream;
}

enum class PIDType : uint16_t
{
    PAT = 0x0000,       // Program Association Table
    CAT = 0x0001,       // Conditional Access Table
    TSDT = 0x0002,      // Transport Stream Description Table
    ICIT = 0x0003,      // IPMP Control Information Table
    NULL_PACKET = 0x1FFF,
};

inline std::ostream & operator << (std::ostream & stream, PIDType pid)
{
    switch (pid)
    {
        case PIDType::PAT:
            stream << "PAT"; return stream;
        case PIDType::CAT:
            stream << "CAT"; return stream;
        case PIDType::TSDT:
            stream << "TSDT"; return stream;
        case PIDType::ICIT:
            stream << "ICIT"; return stream;
        case PIDType::NULL_PACKET:
            stream << "NULL"; return stream;
    }
    stream << "UNKNOWN";
    return stream;
}

enum class ScrambingControl : uint8_t
{
    NotScrambled = 0x0,
    Reserved = 0x1,
    EvenKey = 0x2,
    OddKey = 0x3,
};

} // namespace Media
