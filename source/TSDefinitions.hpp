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

} // namespace Media
