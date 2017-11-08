#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "media/TSDefinitions.hpp"

namespace Media
{

class BitBuffer;

class PSI
{
public:
    PSI()
        : _tableID()
        , _sectionLength()
        , _needMoreData(true)
        , _versionNumber()
        , _current()
        , _sectionNumber()
        , _lastSectionNumber()
        , _crc()
    {
    }

    virtual bool Parse(std::vector<uint8_t> & data) = 0;
    bool NeedMoreData() const { return _needMoreData; }
    virtual bool IsValid() const = 0;
    virtual void DisplayContents() const = 0;

protected:
    TableID _tableID;
    uint16_t _sectionLength;
    bool _needMoreData;
    uint8_t _versionNumber;
    bool _current;
    uint8_t _sectionNumber;
    uint8_t _lastSectionNumber;
    uint32_t _crc;
};


} // namespace Media
